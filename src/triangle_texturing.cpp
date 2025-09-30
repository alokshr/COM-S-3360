#include <stdio.h>
#include <cstdint>
#include "vec3.h"
#include <time.h>

#define EPSILON 0.00001
#define cmpfloat(x,y) (abs((x)-(y)) <= (EPSILON))
#define X 1024
#define Y 1024
#define TX 512
#define TY 256


#define frand() (rand() / (RAND_MAX+1.0))

#define SAMPLES_PER_PIXEL 10

struct triangle {
    vec3 a, b, c;
    vec3 ta, tb, tc;
    vec3 n;
};

vec3 image[Y][X];
vec3 texture[TY][TX];

void load_texture(void) {
    int x, y;
    FILE *f;
    char s[5];
    int r, g, b;

    f = fopen("resources/earth.ppm", "r");

    // Skip over header, hard coded for this example but usually would read these to get info
    fscanf(f, "%s",  s);    // P3
    fscanf(f, "%s",  s);    // TX
    fscanf(f, "%s",  s);    // TY
    fscanf(f, "%s",  s);    // 255

    for (y = 0; y < TY; y++) {
        for (x = 0; x < TX; x++) {
            fscanf(f, "%d %d %d",
                &r,
                &g,
                &b
            );
            texture[y][x] = vec3(r, g, b) / 255.0;
        }
    }
    fclose(f);

    f = fopen("test.ppm", "w");
    fprintf(f, "P3\n%d\n%d\n255\n", TX, TY);
    for (y = 0; y < TY; y++) {
        for (x = 0; x < TX; x++) {
            fprintf(f, "%d %d %d\n",
                (int) (texture[y][x][0] * 255.0),
                (int) (texture[y][x][1] * 255.0),
                (int) (texture[y][x][2] * 255.0)   
            );
        }
    }
    fclose(f);
}

int main(int argc, char const *argv[])
{
    load_texture();

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
        vec3(-1.0, -1.0, -1.0),
        vec3( 1.0,  1.0, -1.0),
        vec3(-1.0,  1.0, -1.0),
        vec3( 0.0,  0.0,  0.0),
        vec3( 1.0,  1.0,  0.0),
        vec3( 0.0,  1.0,  0.0),
        vec3::cross(
            t.b - t.a,
            t.c - t.a
        )
    };

    // Make a triangle
    triangle t2 = {
        vec3(-1.0, -1.0, -1.0),
        vec3( 1.0,  1.0, -1.0),
        vec3(-1.0,  1.0, -1.0),
        vec3( 0.0,  0.0,  0.0),
        vec3( 1.0,  1.0,  0.0),
        vec3( 0.0,  1.0,  0.0),
        vec3::cross(
            t2.b - t2.a,
            t2.c - t2.a
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
                na = vec3::cross(t.c-t.b, p-t.b);
                
                // nb = (a-c) x (p-c);
                nb = vec3::cross(t.a-t.c, p-t.c);
                
                // nc = (b-a) x (p-a);
                nc = vec3::cross(t.b-t.a, p-t.a);

                bary = vec3(
                    (vec3::dot(na, t.n))/t.n.sqmag(),
                    (vec3::dot(nb, t.n))/t.n.sqmag(),
                    (vec3::dot(nc, t.n))/t.n.sqmag()
                );

                // std::cout << na << nb << nc << std::endl;

                // Texture coordinate calculations
                // uv = ta + bary[1](tb - ta) + bary[2](tc - ta)
                vec3 uv = t.a + bary[1]*(t.b - t.a) + bary[2]*(t.c - t.a);

                if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {
                    // image[y][x] += 1.0;
                    int u = (int) (uv.x()*TX);
                    int v = (int) (uv.y()*TY);

                    image[y][x] += texture[v][u];
                }

                // std::cout << "\r[";
                // float percent = (float) (y*X + x*SAMPLES_PER_PIXEL + s)/(X*Y*SAMPLES_PER_PIXEL);
                // int width = (int) (20 * percent);
                
                
            }
        }
    }

    // Write image
    f = fopen("triangle_texturing.ppm", "wb");  // b flag is ignored in non-Windows
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
