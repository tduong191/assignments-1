#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <math.h>

int power(int base, int exp) {
  int result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

void encode(const char* filename, struct ppm_pixel* pixels, int w, int h, int* secret_array) {
  // open file
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
      printf("Error opening file");
      return;
  }
  // write header
  fprintf(fp, "P6\n");   // magic number
  fprintf(fp, "%d %d\n", w, h);   // width and height
  fprintf(fp, "255\n");   // max value

  int length = w*h;
  // write pixels
  for (int i = 0; i < length; i++) {
      // encode the pixels
      struct ppm_pixel encode_pixel = pixels[i];
      int red = encode_pixel.red % 2;
      if (red != secret_array[i*3]) {
        encode_pixel.red += 1;
      }
      int green = encode_pixel.green % 2;
      if (green != secret_array[i*3+1]) {
        encode_pixel.green += 1;
      }
      int blue = encode_pixel.blue % 2;
      if (blue != secret_array[i*3+2]) {
        encode_pixel.blue += 1;
      }
      fwrite(&encode_pixel, sizeof(struct ppm_pixel), 1, fp);
  }
  

  // close file
  fclose(fp);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  // read in ppm file
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);

  // turn 2d array into 1d array
  struct ppm_pixel* pixels_1d = malloc(w * h * sizeof(struct ppm_pixel));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      pixels_1d[i * w + j] = pixels[i][j];
    }
  }

  // encode string
  char* secret_string = malloc(w * h * 3 / 8 * sizeof(char));
  printf("Max number of characters in the images: %d\n", w * h * 3 / 8);
  printf("Enter a message to encode: ");
  fgets(secret_string, w * h * 3 / 8, stdin);
  int* secret_array = malloc(w * h * 3 * sizeof(int));
  for (int i = 0; i < w * h * 3 / 8; i++) {
    int sum = secret_string[i];
    // convert sum to binary
    int* temp = malloc(8 * sizeof(int));
    for (int j = 0; j < 8; j++) {
      temp[j] = sum % 2;
      sum /= 2;
    }
    // add to secret_array
    for (int j = 0; j < 8; j++) {
      secret_array[i*8+j] = temp[7-j];
    }
    // free temp
    free(temp);
  }


  // new file name
  char* new_filename = malloc(strlen(argv[1]) + 7);
  strcpy(new_filename, argv[1]);
  new_filename[strlen(new_filename) - 4] = '\0';
  strcat(new_filename, "-encoded.ppm");
  printf("Writing file %s\n", new_filename);

  // encode image
  encode(new_filename, pixels_1d, w, h, secret_array);

   //free memory
  free(pixels_1d);
  free(secret_array);
  free(secret_string);
  free(new_filename);
  // free 2d array
  for (int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);

  return 0;
}
