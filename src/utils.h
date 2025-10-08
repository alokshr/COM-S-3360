#ifndef UTILS_H
#define UTILS_H
#define _USE_MATH_DEFINES

#include <cmath>
#include "vec3.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = M_PI;

#define EPSILON 0.00001
#define cmpfloat(x,y) (abs((x)-(y)) <= (EPSILON))
#define frand() (rand() / (RAND_MAX+1.0))
#define frand_range(min, max) (min + (max-min)*frand())

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

/**
 * Returns the value clamped between min and max
 * @param value value to clamp
 * @param min lower bound
 * @param max upper bound
 */
inline double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
#endif