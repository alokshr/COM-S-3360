#include <stdio.h>

#define SIZE 512

double image[SIZE][SIZE];
double out[SIZE][SIZE];

const char* image_path = "../../resources/baboon.ascii.pgm";

void load_pgm() {
    FILE *f;
    char s[10];
    int i, j, v;

    f = fopen(image_path, "r");

    fscanf(f, "%s", s);
    // printf("%s\n", s);
    fscanf(f, "%s", s);
    // printf("%s\n", s);
    fscanf(f, "%s", s);
    // printf("%s\n", s);
    fscanf(f, "%s", s);
    // printf("%s\n", s);

    for (j = 0; j < SIZE; j++) {
        for (i = 0; i < SIZE; i++) {
            fscanf(f, "%d", &v);
            image[j][i] = v;
        }
    }
}

void write_pgm(double output[SIZE][SIZE]) {
    FILE *f;
    int i, j;

    f = fopen("convolution.pgm", "w");

    fprintf(f, "P2\n%d %d\n255\n", SIZE, SIZE);

    for (j = 0; j < SIZE; j++) {
        for (i = 0; i < SIZE; i++) {
            fprintf(f, "%d ", (int) output[j][i]);
        }
        if (i % 8 == 0) {
            fprintf(f, "\n");
        }
    }
}

double sobel_x[3][3] = {
    { 1,  0, -1},
    { 2,  0, -2},
    { 1,  0, -1},
};

double sobel_y[3][3] = {
    { 1,  2,  1},
    { 0,  0,  0},
    {-1, -2, -1},
};

double blur[3][3] = {
    { .0625,  .125,  .0625},
    { .125,   .25,   .125},
    { .0625,  .125,  .0625},
};

double sharpen[3][3] = {
    {  0,  -1,   0},
    { -1,   4,  -1},
    {  0,  -1,   0},
};

void convolve(double img[SIZE][SIZE], double k[3][3], double output[SIZE][SIZE]) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        output[i][0] = output[0][i] = output[i][SIZE-1] = output[SIZE-1][i] = 0;
    }

    for (j = 1; j < SIZE - 1; j++) {
        for (i = 1; i < SIZE - 1; i++) {
            output[j][i] = (
                img[j - 1][i - 1] * k[0][0] +
                img[j - 1][i    ] * k[0][1] +
                img[j - 1][i + 1] * k[0][2] +
                img[j    ][i - 1] * k[1][0] +
                img[j    ][i    ] * k[1][1] +
                img[j    ][i + 1] * k[1][2] +
                img[j + 1][i - 1] * k[2][0] +
                img[j + 1][i    ] * k[2][1] +
                img[j + 1][i + 1] * k[2][2]
            );
        }
    }
}

int main(int argc, char const *argv[])
{
    load_pgm();

    // convolve(image, blur, out);
    convolve(image, sharpen, out);

    // convolve(image, sobel_x, out);
    // convolve(image, sobel_y, out);

    write_pgm(out);
    
    return 0;
}
