#include <stdio.h>
#include <cstdint>
#include "vec3.h"
#include <time.h>

#define EPSILON 0.00001
#define cmpfloat(x,y) (abs((x)-(y)) <= (EPSILON))
#define X 1024
#define Y 1024

#define frand() (rand() / (RAND_MAX+1.0))

#define SAMPLES_PER_PIXEL 100

struct triangle {
    vec3 a, b, c;
    vec3 n;
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

    // Make a triangle
    triangle t = {
        vec3(-0.5, -0.5, -1.0),
        vec3(0.0, 0.0, -1.0),
        vec3(-0.25, 0.5, -1.0),
        vec3::cross(
            t.b - t.a,
            t.c - t.a
        )
    };

    // Produce image (plane goes from -1 to 1 in both X and Y directions)
    for (y = 0; y < Y; y++) {
        for (x = 0; x < X; x++) {
            for (s = 0; s < SAMPLES_PER_PIXEL; s++) {
                /*
                switch (s) {
                    case 0:
                        sx = -1.0/(2*X);
                        sy = -1.0/(2*Y);
                        break;
                    case 1:
                        sx = 1.0/(2*X);
                        sy = -1.0/(2*Y);
                        break;
                    case 2:
                        sx = -1.0/(2*X);
                        sy = 1.0/(2*Y);
                        break;
                    case 3:
                        sx = 1.0/(2*X);
                        sy = 1.0/(2*Y);
                        break;
                }
                */

                // Get a random number. Scale it to the dimensions of a pixel.
                // Then, subtract half a pixel.
                sx = (frand() / X) - (1.0/(2.0*X));
                sy = (frand() / Y) - (1.0/(2.0*Y));

                // Get center of a pixel, plus randomness for aliasing
                px = ((double) x/X)*(2.0) - 1 + sx;
                py = ((double) y/Y)*(2.0) - 1 + sy;

                p = vec3(
                    px, 
                    py, 
                    -1.0    // Triangle plane is at -1, usually would need a ray-plane intersection to get this value
                );

                // na = (c-b) x (p-b);
                v1 = t.c - t.b;
                v2 = p - t.b;
                na = vec3::cross(v1, v2);
                
                // nb = (a-c) x (p-c);
                v1 = t.a - t.c;
                v2 = p - t.c;
                nb = vec3::cross(v1, v2);
                
                // nc = (b-a) x (p-a);
                v1 = t.b - t.a;
                v2 = p - t.a;
                nc = vec3::cross(v1, v2);

                bary = vec3(
                    (vec3::dot(na, t.n))/t.n.sqmag(),
                    (vec3::dot(nb, t.n))/t.n.sqmag(),
                    (vec3::dot(nc, t.n))/t.n.sqmag()
                );

                // std::cout << na << nb << nc << std::endl;

                if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {
                    image[y][x] += 1.0;
                }

                // std::cout << "\r[";
                // float percent = (float) (y*X + x*SAMPLES_PER_PIXEL + s)/(X*Y*SAMPLES_PER_PIXEL);
                // int width = (int) (20 * percent);
                
                
            }
        }
    }

    // Write image
    f = fopen("barycentric.ppm", "wb");    // b flag is ignored in non-Windows
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
