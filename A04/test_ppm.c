#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "read_ppm.c"

int main() {
  // load feep-raw and print out the grid of pixels in binary
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
  printf("Testing file feep-raw.ppm: %d %d\n", w, h); 
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      printf("(%3d, %3d, %3d)  ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }

  // free memory
  for (int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);
  return 0;
}

