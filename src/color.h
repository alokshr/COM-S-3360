/**
 * A header file for useful color manipulation
 */
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "mathutils.h"

using color = vec3;

/**
 * Converts a color from float range of [0, 1] to int range [0, 255]
 * @param c color to change
 * @return color converted to int range
 */
color color_float_to_rgb(color c) {
    return color(
        (int) (clamp(c[0], 0, 1) * 255.99),
        (int) (clamp(c[1], 0, 1) * 255.99),
        (int) (clamp(c[2], 0, 1) * 255.99)
    );
}

/**
 * Converts a color from int range of [0, 255] to float range of [0, 1]
 * @param c color to change
 * @return color converted to float range
 */
color color_rgb_to_float(color c) {
    return color(
        clamp(c[0], 0, 255.99) / 255.99,
        clamp(c[1], 0, 255.99) / 255.99,
        clamp(c[2], 0, 255.99) / 255.99
    );
}

/**
 * Calculates RGB illuminance from a given color
 * @param c color to calculate illuminance from
 * @return RGB illuminance
 */
double illuminance(color c) {
    return 0.2126 * c[0] + 0.7152 * c[1] + 0.0722 * c[2];

}
#endif