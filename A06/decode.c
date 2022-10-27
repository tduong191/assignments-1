#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <math.h>

int power(int base, int exp) {
  int result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

// decode odd number to 1 and even number to 0, store in secret_array
void decode(struct ppm_pixel* img, int* secret_array, int length) {
  for (int i = 0; i < length; i++) {
    if (img[i].red % 2 == 0) {
      secret_array[i*3] = 0;
    } else {
      secret_array[i*3] = 1;
    }
    if (img[i].green % 2 == 0) {
      secret_array[i*3+1] = 0;
    } else {
      secret_array[i*3+1] = 1;
    }
    if (img[i].blue % 2 == 0) {
      secret_array[i*3+2] = 0;
    } else {
      secret_array[i*3+2] = 1;
    }
  }
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

  // decode
  int* secret_array = malloc(w * h * 3 * sizeof(int));
  decode(pixels_1d, secret_array, w * h);

  // turn secret_array into a string with every 8 bits as a char
  char* secret_string = malloc(w * h * 3 / 8 * sizeof(char));
  printf("Max number of characters in the images: %d\n", w * h * 3 / 8);
  for (int i = 0; i < w * h * 3 / 8; i++) {
    int sum = 0;
    for (int j = 0; j < 8; j++) {
      sum += secret_array[i * 8 + j] * power(2,(7-j));
    }
    secret_string[i] = sum;
  }

  // print out the secret message
  printf("%s", secret_string);

  //free memory
  free(pixels_1d);
  free(secret_array);
  free(secret_string);
  // free 2d array
  for (int i = 0; i < h; i++) {
    free(pixels[i]);
  }
  free(pixels);
  return 0;
}

