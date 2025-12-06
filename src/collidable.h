#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "renderlib.h"
#include "aabb.h"

class material;

/**
 * A struct used to collect and hold information about collisions
 */
struct collision_hit {
    vec3 point;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    double u, v;
    bool front_face;

    /** Sets the hit record normal vector.
     * NOTE: the parameter `outward_normal` is assumed to have unit length.
     */
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = vec3::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/**
 * A virtual class for interfacing with objects affected by light
 */
class collidable {
    public:
        virtual ~collidable() = default;

        /**
         * Returns if a ray collides with this collidable object and information about the collision
         * @param r ray to check
         * @param ray_t interval of ray to check
         * @param rec place to collect collision info
         * @return true if ray collides, false if ray doesn't collide
         */
        virtual bool hit(const ray& r, interval ray_t, collision_hit& rec) const = 0;

        /**
         * Returns the bounding box of this collidable object
         * @return bounding box
         */
        virtual aabb bounding_box() const = 0;
};
#endif