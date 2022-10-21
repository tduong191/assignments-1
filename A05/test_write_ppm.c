#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  char* filename = "feep-raw.ppm";
  struct ppm_pixel* pixels = read_ppm(filename, &w, &h);
  write_ppm("feep-raw-test.ppm", pixels, w, h);

  free(pixels);
  pixels = read_ppm("feep-raw-test.ppm", &w, &h);

  printf("Testing file %s: %d %d\n", filename, w, h); 
  for (int r = 0; r < h; r++) {
    for (int c = 0; c < w; c++) {
       struct ppm_pixel p = pixels[r*w+c]; 
       printf("(%u,%u,%u) ", p.red, p.green, p.blue); 
    }
    printf("\n");
  }

  free(pixels);
  return 0;
}
