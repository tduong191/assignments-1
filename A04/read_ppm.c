#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  // open file
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Error opening file");
    return NULL;
  }

  char line[100];
  fgets(line, sizeof(line), fp);
  if (strcmp(line, "P6\n") != 0) {
    return NULL;
  } else {
    // skip space
    fgets(line, sizeof(line), fp);
    // skip comments
    fgets(line, sizeof(line), fp);
    while (line[0] == '#') {
      fgets(line, sizeof(line), fp);
    }
    // get width and height
    sscanf(line, "%d %d", w, h);
    // skip max value
    fgets(line, sizeof(line), fp);
  }

  // allocate memory for 2d array
  struct ppm_pixel **pixels = malloc(*h * sizeof(struct ppm_pixel *));
  for (int i = 0; i < *h; i++) {
    pixels[i] = malloc(*w * sizeof(struct ppm_pixel));
  }

  // check if there is enough memory
  if (pixels == NULL) {
    printf("Error: not enough memory\n");
    return NULL;
  }

  // read in pixels by line and store in 2d array
  for (int i = 0; i < *h; i++) {
    for (int j = 0; j < *w; j++) {
      fread(&pixels[i][j], sizeof(struct ppm_pixel), 1, fp);
    }
  }

  // close file
  fclose(fp);

  return pixels;
}
