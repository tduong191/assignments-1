#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include "math.h"
#include <stdbool.h> 

struct thread_data {
  int id;
  int start_i;
  int end_i;
  int start_j;
  int end_j;
  struct ppm_pixel** pixels;
  int** member_count;
  bool** member; 
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int* max_count;
  pthread_mutex_t* lock;
  pthread_barrier_t* barrier;
};

void *calculate_mandelbrot(void *threadarg) {
  struct thread_data *data = (struct thread_data *) threadarg;
  printf("Thread %d) Sub-image block cols (%d, %d) to rows (%d, %d)\n", data->id, data->start_j, data->end_j, data->start_i, data->end_i);

  // Step 1: Determine mandelbrot set membership
  for (int i = data->start_i; i < data->end_i; i++) {
    for (int j = data->start_j; j < data->end_j; j++) {
      float xfarc = (float) j / (float) data->size;
      float yfarc = (float) i / (float) data->size;
      float x0 = data->xmin + (data->xmax - data->xmin) * xfarc;
      float y0 = data->ymin + (data->ymax - data->ymin) * yfarc;
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
        // set membership at (row,col) to false
        data->member[i][j] = false;
      } else {
        // set membership at (row,col) to true
        data->member[i][j] = true;
      }
    }
  }

  // Step 2: Compute visited counts
  for (int i = data->start_i; i < data->end_i; i++) {
    for (int j = data->start_j; j < data->end_j; j++) {
      if (data->member[i][j] == true) {
        continue;
      } 
      float xfrac = (float) j / (float) data->size;
      float yfrac = (float) i / (float) data->size;
      float x0 = data->xmin + (data->xmax - data->xmin) * xfrac;
      float y0 = data->ymin + (data->ymax - data->ymin) * yfrac;
      float x = 0.0;
      float y = 0.0;
      int iteration = 0;
      while (x * x + y * y < 2 * 2) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        int yrow = round((y - data->ymin) * data->size / (data->ymax - data->ymin));
        int xcol = round((x - data->xmin) * data->size / (data->xmax - data->xmin));
        if (yrow >= 0 && yrow < data->size && xcol >= 0 && xcol < data->size) {
          pthread_mutex_lock(data->lock); 
          data->member_count[yrow][xcol]++;
          pthread_mutex_unlock(data->lock);
          if (data->member_count[yrow][xcol] > *data->max_count) {
            pthread_mutex_lock(data->lock);
            *data->max_count = data->member_count[yrow][xcol];
            pthread_mutex_unlock(data->lock);
          }
        }
      }
    }
  }

  pthread_barrier_wait(data->barrier);
  // Step 3: Compute colors
  float gamma = 0.681;
  float factor = 1.0 / gamma;
  for (int i = data->start_i; i < data->end_i; i++) {
    for (int j = data->start_j; j < data->end_j; j++) {
      float value = 0;
      int count = data->member_count[i][j];
      if (count > 0) {
        value = log(count) / log(*data->max_count);
        value = pow(value, factor);
      }
      data->pixels[i][j].red = (int) (255 * value);
      data->pixels[i][j].green = (int) (255 * value);
      data->pixels[i][j].blue = (int) (255 * value);
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
  // compute image

  // create an array of boolean for member check 
  bool** member = malloc(size * sizeof(bool*));
  for (int i = 0; i < size; i++) {
    member[i] = malloc(size * sizeof(bool));
  }

  // create an array of integers to count the number of times each pixel is in the set
  int** member_count = malloc(size * sizeof(int*));
  for (int i = 0; i < size; i++) {
    member_count[i] = malloc(size * sizeof(int));
    // initialize to 0
    for (int j = 0; j < size; j++) {
      member_count[i][j] = 0;
    }
  }

  // create the pixels
  struct ppm_pixel** pixels;
  pixels = malloc(size * sizeof(struct ppm_pixel*));
  for (int i = 0; i < size; i++) {
    pixels[i] = malloc(size * sizeof(struct ppm_pixel));
  }

  // mutex lock
  pthread_mutex_t lock;
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init has failed\n");
    return 1;
  } 
  
  // barrier
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, 4);

  // declare threads
  pthread_t threads[4];
  struct thread_data thread_data_array[4];

  // max count
  int max_count = 0;

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
    thread_data_array[i].member_count = member_count;
    thread_data_array[i].member = member;
    thread_data_array[i].size = size;
    thread_data_array[i].xmin = xmin;
    thread_data_array[i].xmax = xmax;
    thread_data_array[i].ymin = ymin;
    thread_data_array[i].ymax = ymax;
    thread_data_array[i].maxIterations = maxIterations;
    thread_data_array[i].max_count = &max_count;
    thread_data_array[i].lock = &lock;
    thread_data_array[i].barrier = &barrier;
    pthread_create(&threads[i], NULL, calculate_mandelbrot, (void*) &thread_data_array[i]);
  }

  // join threads
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // end time
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("Computed mandelbrot set (%dx%d) in %f seconds.\n", size, size, time_taken);

  // destroy mutex lock
  pthread_mutex_destroy(&lock);

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

  // free the member count
  for (int i = 0; i < size; i++) {
    free(member_count[i]);
  }
  free(member_count);

  // free the member
  for (int i = 0; i < size; i++) {
    free(member[i]);
  }
  free(member);

  return 0;
}
