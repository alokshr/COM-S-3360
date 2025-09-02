#include <iostream>
#include <fstream>
#include <cstdint>

#define X 1024
#define Y 760

#define R 0
#define G 1
#define B 2

uint8_t raster[X][Y][3];

struct vec3_t {

};

// Using .ppm image file format, render in Irfanview
int main(int argc, char** argv) {

    int x, y;
    FILE *f;

    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            raster[y][x][R] = (x * 255) / X;
            raster[y][x][G] = (y * 255) / X;
            raster[y][x][B] = 255 - ((y * 255) / X);
        }
    }

    f = fopen("gradient.ppm", "wb");    // b flag is ignored in non-Windows
                                        // In Windows, "binary mode" suppresses carriage return after \n

    fprintf(f, "P6 %d %d 255 ", X, Y);
    fwrite(raster, sizeof(uint8_t) * 3, X*Y, f);

    return 0;
}