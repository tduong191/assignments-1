#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct thread_data {
  int id;
  int start_i;
  int end_i;
  int start_j;
  int end_j;
  struct ppm_pixel** pixels;
  struct ppm_pixel* pallet;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
};

void *calculate_mandelbrot(void *threadarg) {
  struct thread_data *data = (struct thread_data *) threadarg;
  printf("Thread %d) Sub-image block cols (%d, %d) to rows (%d, %d)\n", data->id, data->start_j, data->end_j, data->start_i, data->end_i);
  for (int i = data->start_i; i < data->end_i; i++) {
    for (int j = data->start_j; j < data->end_j; j++) {
      float x0 = data->xmin + (data->xmax - data->xmin) * j / data->size;
      float y0 = data->ymin + (data->ymax - data->ymin) * i / data->size;
      float x = 0.0;
      float y = 0.0;
      int iteration = 0;
      while (x * x + y * y < 2 * 2 && iteration < data->maxIterations) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iteration++;
      }
      if (iteration < data->maxIterations) {
        data->pixels[i][j] = data->pallet[iteration];
      } else {
        data->pixels[i][j].red = 0;
        data->pixels[i][j].green = 0;
        data->pixels[i][j].blue = 0;
      }
    }
  }
  printf("Thread %d) finished\n", data->id);
  return (void *) NULL;
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
  srand(time(0));
  struct ppm_pixel* pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
  for (int i = 0; i < maxIterations; i++) {
    pallet[i].red = rand() % 256;
    pallet[i].green = rand() % 256;
    pallet[i].blue = rand() % 256;
  }

  // new ppm file
  struct ppm_pixel** pixels;

  // create the pixels
  pixels = malloc(size * sizeof(struct ppm_pixel*));
  for (int i = 0; i < size; i++) {
    pixels[i] = malloc(size * sizeof(struct ppm_pixel));
  }

  // declare threads
  pthread_t threads[4];
  struct thread_data thread_data_array[4];

  // create threads
  clock_t t;
  t = clock();
  for (int i = 0; i < 4; i++) {
    thread_data_array[i].id = i;
    if (i < 2) {
      thread_data_array[i].start_i = 0;
      thread_data_array[i].end_i = size / 2;
    } else {
      thread_data_array[i].start_i = size / 2;
      thread_data_array[i].end_i = size;
    }

    if (i % 2 == 0) {
      thread_data_array[i].start_j = 0;
      thread_data_array[i].end_j = size / 2;
    } else {
      thread_data_array[i].start_j = size / 2;
      thread_data_array[i].end_j = size;
    }
    thread_data_array[i].pixels = pixels;
    thread_data_array[i].pallet = pallet;
    thread_data_array[i].size = size;
    thread_data_array[i].xmin = xmin;
    thread_data_array[i].xmax = xmax;
    thread_data_array[i].ymin = ymin;
    thread_data_array[i].ymax = ymax;
    thread_data_array[i].maxIterations = maxIterations;
    pthread_create(&threads[i], NULL, calculate_mandelbrot, (void*) &thread_data_array[i]);
  }

  // join threads
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("Computed mandelbrot set (%dx%d) in %f seconds.\n", size, size, time_taken);

  // create a new file name with size and time
  char filename[100];
  int currtime = time(0);
  sprintf(filename, "mandelbrot_%d_%d.ppm", size, currtime);
  printf("Writing to file %s.\n", filename);

  // write the file
  write_ppm_2d(filename, pixels, size, size);

  // free the pixels
  for (int i = 0; i < size; i++) {
    free(pixels[i]);
  }
  free(pixels);

  // free the pallet
  free(pallet);
}
