#include <stdio.h>
// #include <cstdint>
#include <time.h>

#include "../vec3.h"
#include "../ray.h"

#define EPSILON 0.00001
#define cmpfloat(x,y) (abs((x)-(y)) <= (EPSILON))
#define X 1024
#define Y 1024
#define TX 512
#define TY 256

#define frand() (rand() / (RAND_MAX+1.0))

#define SAMPLES_PER_PIXEL 10

struct circle {
    vec3 center;
    double radius;
};

vec3 image[Y][X];



int main(int argc, char const *argv[])
{
    int x, y, s;
    double px, py;
    vec3 p;
    vec3 v1;
    vec3 v2;
    vec3 na, nb, nc;
    vec3 bary;
    double sx, sy;

    FILE *f;

    srand(time(NULL));

    circle c = {
        vec3(1, 0, 0),
        10
    };

    // Produce image (plane goes from -1 to 1 in both X and Y directions)
    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            for (s = 0; s < SAMPLES_PER_PIXEL; s++) {
                // Get a random number. Scale it to the dimensions of a pixel.
                // Then, subtract half a pixel.
                sx = (frand() / X) - (1.0/(2.0*X));
                sy = (frand() / Y) - (1.0/(2.0*Y));

                // Get center of a pixel, plus randomness for aliasing
                px = ((double) x/X)*(2.0) - 1 + sx;
                py = ((double) y/Y)*(2.0) - 1 + sy;

                ray 

                image[y][x] += ray_color();
            }
        }
    }

    // Write image
    f = fopen("circle.ppm", "wb");  // b flag is ignored in non-Windows
                                    // In Windows, "binary mode" suppresses carriage return after \n

    fprintf(f, "P3 %d %d %d\n", X, Y, 255);
    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            fprintf(f, "%d %d %d ",
                (unsigned char) (image[y][x][0] * 255.99/SAMPLES_PER_PIXEL),
                (unsigned char) (image[y][x][1] * 255.99/SAMPLES_PER_PIXEL),
                (unsigned char) (image[y][x][2] * 255.99/SAMPLES_PER_PIXEL)
            );
        }
    }
    return 0;
}
