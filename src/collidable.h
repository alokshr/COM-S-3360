#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "renderlib.h"

class material;

struct collision_hit {
    vec3 point;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    bool front_face;

    /** Sets the hit record normal vector.
     * NOTE: the parameter `outward_normal` is assumed to have unit length.
     */
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = vec3::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class collidable {
    public:
        virtual ~collidable() = default;

        virtual bool hit(const ray& r, interval ray_t, collision_hit& rec) const = 0;
};
#endif