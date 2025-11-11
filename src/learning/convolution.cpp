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

void convolve(double k[3][3]) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        out[i][0] = out[0][i] = out[i][SIZE-1] = out[SIZE-1][i] = 0;
    }

    for (j = 1; j < SIZE - 1; j++) {
        for (i = 1; i < SIZE - 1; i++) {
            out[j][i] = (
                image[j - 1][i - 1] * k[0][0] +
                image[j - 1][i    ] * k[0][1] +
                image[j - 1][i + 1] * k[0][2] +
                image[j    ][i - 1] * k[1][0] +
                image[j    ][i    ] * k[1][1] +
                image[j    ][i + 1] * k[1][2] +
                image[j + 1][i - 1] * k[2][0] +
                image[j + 1][i    ] * k[2][1] +
                image[j + 1][i + 1] * k[2][2]
            );
        }
    }
}

int main(int argc, char const *argv[])
{
    load_pgm();

    convolve(sobel_x);
    // convolve(sobel_y);

    write_pgm(out);
    
    return 0;
}
