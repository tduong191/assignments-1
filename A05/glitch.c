#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void glitch(const char* filename, struct ppm_pixel** pixels, int w, int h) {
  // open file
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
      printf("Error opening file");
      return;
  }
  // write header
  fprintf(fp, "P6\n");   // magic number
  fprintf(fp, "\n");     // space
  fprintf(fp, "%d %d\n", w, h);   // width and height
  fprintf(fp, "255\n");   // max value

  // write pixels
  for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
          // glitch the pixels
          struct ppm_pixel glitch_pixel = pixels[i][j];
          glitch_pixel.red = glitch_pixel.red << (rand() % 2);
          glitch_pixel.green = glitch_pixel.green << (rand() % 2);
          glitch_pixel.blue = glitch_pixel.blue << (rand() % 2);
          fwrite(&glitch_pixel, sizeof(struct ppm_pixel), 1, fp);
      }
  }

  // close file
  fclose(fp);
}

void cleanup(struct ppm_pixel** img, int width, int height) {
  for (int i = 0; i < height; i++) {
    free(img[i]);
  }
  free(img);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: glitch <file.ppm>\n");
    return 0;
  }

  // todo: your code here
  int w, h;
  char* filename = argv[1];

  // read in image
  struct ppm_pixel** pixels = read_ppm_2d(filename, &w, &h);
  printf("Reading %s with width %d and height %d\n", filename, w, h);
  // new file name
  char* new_filename = malloc(strlen(filename) + 7);
  strcpy(new_filename, filename);
  new_filename[strlen(new_filename) - 4] = '\0';
  strcat(new_filename, "-glitch.ppm");
  printf("Writing file %s\n", new_filename);

  // glitch image
  glitch(new_filename, pixels, w, h);

  // free memory
  cleanup(pixels, w, h);
  free(new_filename);


  return 0;
}

