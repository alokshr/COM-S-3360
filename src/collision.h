#ifndef COLLISION_H
#define COLLISION_H

#include "ray.h"

/**
 * Returns true if the given ray intersects the sphere
 * @param center 
 */
double hit_sphere(const vec3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    double a = r.direction().sqmag();
    double h = vec3::dot(r.direction(), oc);
    double c = oc.sqmag() - radius*radius;
    double discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }
}
#endif