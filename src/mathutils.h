#ifndef UTILS_H
#define UTILS_H
#define _USE_MATH_DEFINES

#include <cmath>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = M_PI;

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

/**
 * Generates a random number between 0 inclusive and 1 exclusive
 */
inline double random() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/**
 * Returns a random number between min inclusive and max exclusive.
 * @param min lower bound of random number generated, inclusive
 * @param max upper bound of random number generated, exclusive
 */
inline double random(double min, double max) {
    return min + (max-min)*random();
}

#endif