#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void cleanup(struct ppm_pixel** img, int width, int height) {
  for (int i = 0; i < height; i++) {
    free(img[i]);
  }
  free(img);
}

int main(int argc, char** argv) {
  int w, h;
  char* filename = "feep-raw.ppm";

  struct ppm_pixel** pixels = read_ppm_2d(filename, &w, &h);
  write_ppm_2d("feep-raw-test.ppm", pixels, w, h);
  cleanup(pixels, w, h);

  pixels = read_ppm_2d("feep-raw-test.ppm", &w, &h);
  printf("Testing file %s: %d %d\n", filename, w, h); 
  for (int r = 0; r < h; r++) {
    for (int c = 0; c < w; c++) {
       struct ppm_pixel p = pixels[r][c]; 
       printf("(%u,%u,%u) ", p.red, p.green, p.blue); 
    }
    printf("\n");
  }

  cleanup(pixels, w, h);
  return 0;
}

