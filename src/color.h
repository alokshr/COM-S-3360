/**
 * A header file for useful color manipulation
 */
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using color = vec3;

/**
 * Converts a color from float range of [0, 1] to int range [0, 255]
 * @param vec3 vector to change
 */
color color_float_to_rgb(color c) {
    return color(
        (int) (c[0] * 255.99),
        (int) (c[1] * 255.99),
        (int) (c[2] * 255.99)
    );
}

/**
 * Converts a color from int range of [0, 255] to float range of [0, 1]
 * @param vec3 vector to change
 */
color color_rgb_to_float(color c) {
    return color(
        c[0] / 255.99,
        c[1] / 255.99,
        c[2] / 255.99
    );
}
#endif