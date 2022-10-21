#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  // todo: your code here

  // array of 8x8 pixels
  char* pixels[8][8];

  // create a bit mask for the leftmost bit
  unsigned long mask = 0x1ul << 63;

  // loop through the 8x8 array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // if the bit is 1, set the pixel to 1
      if (img & mask) {
        pixels[i][j] = "@";
      } else {
        pixels[i][j] = " ";
      }
      // shift the mask to the right
      mask = mask >> 1;
    }
  }

  // print the array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%s", pixels[i][j]);
    }
    printf("\n");
  }

  return 0;
}
