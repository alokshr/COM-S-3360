#ifndef UTILS_H
#define UTILS_H

#include "vec3.h"

double lerp(double a, double b, double t) {
    return (1-t)*a + t*b;
}

vec3 lerp(vec3 v1, vec3 v2, double t) {
    return (1-t)*v1 + t*v2;
}

#endif