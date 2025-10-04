#ifndef COLLISION_H
#define COLLISION_H

#include "ray.h"

/**
 * Returns true if the given ray intersects the sphere
 * @param center 
 */
bool hit_sphere(const vec3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    double a = vec3::dot(r.direction(), r.direction());
    double b = -2.0 * vec3::dot(r.direction(), oc);
    double c = vec3::dot(oc, oc) - radius*radius;
    double discriminant = b*b - 4*a*c;

    return discriminant >= 0;
}
#endif