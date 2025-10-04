#ifndef RAY_H
#define RAY_H

#include "vec3.h"
/**
 * A class for representing rays in 3D space
 */
class ray {
    public:
        ray() {};
        ray(const vec3& origin, const vec3& direction): orig(origin), dir(direction) {};

        const vec3& origin() const  { return orig; }
        const vec3& direction() const { return dir; }

        vec3 at(double t) const {
            return orig + t*dir;
        }

    private:
        vec3 orig;
        vec3 dir;
};
#endif