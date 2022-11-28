#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

void calculate_mandelbrot(int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, struct ppm_pixel** pixels, struct ppm_pixel* pallet, int start_i, int end_i, int start_j, int end_j) {
  printf("%d) Sub-image block cols (%d, %d) to rows (%d, %d)\n", getpid(), start_j, end_j, start_i, end_i);
  for (int i = start_i; i < end_i; i++) {
    for (int j = start_j; j < end_j; j++) {
      float x0 = xmin + (xmax - xmin) * j / size;
      float y0 = ymin + (ymax - ymin) * i / size;
      float x = 0.0;
      float y = 0.0;
      int iteration = 0;
      while (x * x + y * y < 2 * 2 && iteration < maxIterations) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iteration++;
      }
      if (iteration < maxIterations) {
        pixels[i][j] = pallet[iteration];
      } else {
        pixels[i][j].red = 0;
        pixels[i][j].green = 0;
        pixels[i][j].blue = 0;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  // compute image
  srand(time(0));
  struct ppm_pixel* pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for (int i = 0; i < maxIterations; i++) {
    pallet[i].red = rand() % 256;
    pallet[i].green = rand() % 256;
    pallet[i].blue = rand() % 256;
  }

  // allocate memory for pixels using shared memory
  int shmid = shmget(IPC_PRIVATE, size * sizeof(struct ppm_pixel*), IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }
  struct ppm_pixel** pixels = shmat(shmid, NULL, 0);
  int* sharedID = malloc(sizeof(int)*size);
  for (int i = 0; i < size; i++) {
    int shmid1 = shmget(IPC_PRIVATE, size * sizeof(struct ppm_pixel), IPC_CREAT | 0666);
    pixels[i] = shmat(shmid1, NULL, 0);
    sharedID[i] = shmid1;
  }
  if (pixels == (void*)-1) {
    perror("shmat");
    exit(1);
  }

  // create processes
  pid_t pid;
  int status;
  clock_t t;
  t = clock();
  int start_i, end_i, start_j, end_j;
  start_j = 0;
  end_j = size / 2;
  pid = fork();
  if (pid == 0) {
    start_i = size / 2;
    end_i = size;
    pid = fork();
    if (pid == 0) {
      start_j = size / 2;
      end_j = size;
      printf("Launching child process: %d.\n", getpid());
      calculate_mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pixels, pallet, start_i, end_i, start_j, end_j);
      printf("Child process %d complete.\n", getpid());
      exit(0);
    } 
    else {
      wait(&status);
      printf("Launching child process: %d.\n", getpid());
      calculate_mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pixels, pallet, start_i, end_i, start_j, end_j);
      printf("Child process %d complete.\n", getpid());
    }
    exit(0);
  } else {
    start_i = 0;
    end_i = size / 2;
    pid = fork();
    if (pid == 0) {
      start_j = size / 2;
      end_j = size;
      printf("Launching child process: %d.\n", getpid());
      calculate_mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pixels, pallet, start_i, end_i, start_j, end_j);
      printf("Child process %d complete.\n", getpid());
      exit(0);
    } 
    else {
      wait(&status);
      printf("Launching child process: %d.\n", getpid());
      calculate_mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pixels, pallet, start_i, end_i, start_j, end_j);
      printf("Child process %d complete.\n", getpid());
    }
  } 

  printf("Child process %d complete.\n", getpid());
  wait(&status);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("Computed mandelbrot set (%dx%d) in %f seconds.\n", size, size, time_taken);
  exit(0);
  char filename[100];
  int currtime = time(0);
  sprintf(filename, "mandelbrot_%d_%d.ppm", size, currtime);
  printf("Writing to file %s.\n", filename);

  // write the file
  write_ppm_2d(filename, pixels, size, size);

  // detach shared memory
  for (int i = 0; i < size; i++) {
    shmdt(pixels[i]);
  }
  shmdt(pixels);
  
  // delete shared memory
  for (int i = 0; i < size; i++) {
    shmctl(sharedID[i], IPC_RMID, NULL);
  }
  shmctl(shmid, IPC_RMID, NULL);
  
  // free sharedID
  free(sharedID);

  // free pallet
  free(pallet);
  return 0;
}
