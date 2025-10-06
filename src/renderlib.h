/**
 * A standard library with helper functions for handling general rendering calculations
 */
#ifndef RENDERLIB_H
#define RENDERLIB_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "vec3.h"
#include "color.h"
#include "ray.h"

#define EPSILON 0.00001
#define cmpfloat(x,y) (abs((x)-(y)) <= (EPSILON))
#define frand() (rand() / (RAND_MAX+1.0))
#define frand(min, max) (min + (max-min)*frand())


const double infinity = std::numeric_limits<double>::infinity();
const double pi = M_PI;

#include "interval.h"


/**
 * Converts degrees to radians
 * @param degrees value in degrees
 * @return value in radians
 */
inline double d2r(double degrees) {
    return degrees * pi/180.0;
}

/**
 * Converts radians to degrees
 * @param radians value in radians
 * @return value in degrees
 */
inline double r2d(double radians) {
    return radians * 180.0/pi;
}

/**
 * Returns a linearly interpolated point from a to b
 * @param a start
 * @param b end
 * @param t value in range of 0-1 inclusive
 */
inline double lerp(double a, double b, double t) {
    return (1-t)*a + t*b;
}

/**
 * Returns a linearly interpolated vec3 from a to b
 * @param a start vec3
 * @param b end vec3
 * @param t value in range of 0-1 inclusive
 */
inline vec3 lerp(vec3 v1, vec3 v2, double t) {
    return (1-t)*v1 + t*v2;
}
#endif