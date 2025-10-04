#include "../image.h"

#define X 1024
#define Y 760

// Create blank image
image img(Y, std::vector<color>(X, color()));

// Using .ppm image file format, render in Irfanview
int main(int argc, char** argv) {

    int x, y;

    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            img[y][x] = color(
                ((double) x) / X,
                ((double) y) / Y,
                1 - (((double) y) / Y)
            );
        }
    }

    output_ppm_image(img, "image.ppm");

    return 0;
}