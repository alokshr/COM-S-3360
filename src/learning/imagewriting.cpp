#include <iostream>
#include <fstream>
#include <cstdint>
#include "../vec3.h"

#define X 1024
#define Y 760

#define R 0
#define G 1
#define B 2

vec3 raster[Y][X];

// Using .ppm image file format, render in Irfanview
int main(int argc, char** argv) {

    int x, y;
    FILE *f;

    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            raster[y][x] = vec3(
                (x * 255) / X,
                (y * 255) / X,
                255 - ((y * 255) / X)
            );
            
        }
    }

    f = fopen("gradient.ppm", "wb");    // b flag is ignored in non-Windows
                                        // In Windows, "binary mode" suppresses carriage return after \n

    fprintf(f, "P6 %d %d 255 ", X, Y);
    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            uint8_t buffer[] = {
                (uint8_t) raster[y][x].e[0],
                (uint8_t) raster[y][x].e[1],
                (uint8_t) raster[y][x].e[2]
            };

            fwrite(buffer, sizeof(uint8_t)*3, 1, f);
        }
    }

    return 0;
}