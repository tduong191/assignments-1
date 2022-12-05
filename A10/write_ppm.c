#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
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
            fwrite(&pixels[i][j], sizeof(struct ppm_pixel), 1, fp);
        }
    }

    // close file
    fclose(fp);

    return;
}