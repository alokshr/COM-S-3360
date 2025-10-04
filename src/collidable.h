#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "ray.h"

struct collision_hit {
    vec3 point;
    vec3 normal;
    double t;
};

class collidable {
    public:
        virtual ~collidable() = default;

        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, collision_hit& rec) const = 0;
};
#endif