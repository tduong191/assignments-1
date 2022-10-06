#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "read_ppm.c"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ascii_image <file.ppm>\n");
        return 1;
    }

    int width, height;
    struct ppm_pixel** image = read_ppm_2d(argv[1], &width, &height);

    if (image == NULL) {
        printf("Error reading file %s. Filename is not valid or there is no memory available.\n", argv[1]);
        return 1;
    }

    printf("Reading file %s with width %d and height %d.\n", argv[1], width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = image[i][j].red;
            int g = image[i][j].green;
            int b = image[i][j].blue;
            int avg = (r + g + b) / 3;
            if (avg < 26) {
                printf("@");
            } else if (avg < 51) {
                printf("#");
            } else if (avg < 76) {
                printf("%%");
            } else if (avg < 101) {
                printf("*");
            } else if (avg < 126) {
                printf("o");
            } else if (avg < 151) {
                printf(";");
            } else if (avg < 176) {
                printf(":");
            } else if (avg < 201) {
                printf(",");
            } else if (avg < 226) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    // free the memory
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
    return 0;
}






