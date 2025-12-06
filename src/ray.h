#ifndef RAY_H
#define RAY_H

#include "vec3.h"
/**
 * A class for representing rays in 3D space
 */
class ray {
    public:

        /**
         * Creates a default ray
         */
        ray() {};

        /**
         * Creates a ray from an origin point and a direction, with a given time
         * @param origin origin point of ray
         * @param direction direction of ray
         * @param time time of ray
         */
        ray(const vec3& origin, const vec3& direction, double time): orig(origin), dir(direction), t(time) {};

        /**
         * Creates a ray from an origin point and a direction
         * @param origin origin point of ray
         * @param direction direction of ray 
         */
        ray(const vec3& origin, const vec3& direction): ray(origin, direction, 0) {};

        /**
         * Returns the origin point of this ray
         * @return origin point
         */
        const vec3& origin() const  { return orig; }

        /**
         * Returns the direction of this ray
         * @return direction
         */
        const vec3& direction() const { return dir; }

        /**
         * Returns the time of this ray
         * @return time
         */
        double time() const { return t; }

        /**
         * Returns the point along the ray
         * @param t time/percentage along the ray
         * @return position at given t
         */
        vec3 at(double t) const {
            return orig + t*dir;
        }

    private:
        /**
         * The origin of this ray
         */
        vec3 orig;

        /**
         * The direction of this ray
         */
        vec3 dir;

        /**
         * The time that this ray is shot at
         */
        double t;
};
#endif