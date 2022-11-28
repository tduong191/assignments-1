#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include "string.h"

void calculate_mandelbrot(int size, float xmin, float xmax, float ymin, float ymax, int maxIterations, struct ppm_pixel** pixels, struct ppm_pixel* pallet, int start_i, int end_i, int start_j, int end_j) {
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

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  srand(time(0));
  // generate the pallet
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
  clock_t t;
  t = clock();
  // calculate the mandelbrot
  calculate_mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pixels, pallet, 0, size, 0, size);
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
