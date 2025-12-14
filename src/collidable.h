#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "renderlib.h"
#include "aabb.h"
#include "quaternion.h"

class material;

/**
 * A struct used to collect and hold information about collisions
 */
struct collision_hit {
    vec3 point;                     // collision point
    vec3 normal;                    // normal of collision
    std::shared_ptr<material> mat;  // material from collision
    double t;                       // t value of ray that collided
    double u, v;                    // u-v values of the collision's texture
    bool front_face;                // used to determine if collision happened "inside" or "outside"

    /** Sets the hit record normal vector
     * @param r incoming ray
     * @param outward_normal unit vector normal of the collision
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

        /**
         * Returns the pdf value associated with this collidable and the given incoming ray
         * @param origin origin of incoming ray
         * @param direction direction of incoming ray
         * @return pdf value of ray
         */
        virtual double pdf_value(const vec3& origin, const vec3& direction) const {
            return 0.0;
        }

        /**
         * Returns a random direction towards this collidable
         * @param origin point to come from
         * @return direction to collidable
         */
        virtual vec3 random(const vec3& origin) const {
            return vec3(1, 0, 0);
        }
};

/**
 * A class for translating collidable objects around in 3D space
 */
class translate : public collidable {
    public:
        /**
         * Creates a translated copy of an existing collidable object
         * @param obj original object
         * @param offset translation offset from original object
         */
        translate(shared_ptr<collidable> obj, vec3 offset):obj(obj), offset(offset) {
            bbox = obj->bounding_box() + offset;
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            // Offset our ray to mimic moving this object 
            ray offset_r(r.origin() - offset, r.direction(), r.time());

            // Use object's hit function with offset ray
            if (!obj->hit(offset_r, ray_t, rec)) return false;

            // Make sure to update hit point with offset
            rec.point += offset;

            return true;
        }

        aabb bounding_box() const { return bbox; }

    private:
        /**
         * The object affected by this translation
         */
        shared_ptr<collidable> obj;
        
        /**
         * The offset of this translation
         */
        vec3 offset;
        
        /**
         * The updated bounding box after being moved by this translation
         */
        aabb bbox;
};


/**
 * A class for translating collidable objects around in 3D space
 */
class rotate : public collidable {
    public:
        /**
         * Creates a translated copy of an existing collidable object
         * @param obj original object
         * @param offset translation offset from original object
         */
        rotate(shared_ptr<collidable> obj, vec3 axis, double degrees): obj(obj), axis(axis.normalize()), degrees(degrees) {}

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            // Transform the ray into object space
            vec3 orig = rotate_by_axis(r.origin(), axis, -degrees);
            vec3 dir = rotate_by_axis(r.direction(), axis, -degrees);

            ray rotated_r(orig, dir, r.time());

            if (!obj->hit(rotated_r, ray_t, rec)) {
                return false;
            }

            // Transform normal and collision point back to world space
            rec.normal = rotate_by_axis(rec.normal, axis, degrees);
            rec.point = rotate_by_axis(rec.point, axis, degrees);

            return true;
        }

        aabb bounding_box() const { return bbox; }

    private:
        /**
         * The object affected by this rotation
         */
        shared_ptr<collidable> obj;
        
        /**
         * The axis this rotation is based around
         */
        vec3 axis;

        /**
         * The amount to rotate by in degrees
         */
        double degrees;
        
        /**
         * The updated bounding box after being moved by this rotation
         */
        aabb bbox;
};

/**
 * A class for scaling collidables
 */
class scale : public collidable {
    public:
        /**
         * Creates a scaled version of a given collidable object
         * @param obj object to scale
         * @param s amount to scale by
         */
        scale(shared_ptr<collidable> obj, const vec3& s)
            : object(obj), scale_factor(s),
            inv_scale(1.0 / s.x(), 1.0 / s.y(), 1.0 / s.z())
        {
            // Scale bounding box
            bbox = object->bounding_box();

            vec3 min = vec3(bbox.x.min, bbox.y.min, bbox.z.min);
            vec3 max = vec3(bbox.x.max, bbox.x.max, bbox.x.max);

            min = vec3(min.x() * s.x(), min.y() * s.y(), min.z() * s.z());
            max = vec3(max.x() * s.x(), max.y() * s.y(), max.z() * s.z());

            bbox = aabb(min, max);
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            // Transform ray into object space
            ray scaled_ray(
                r.origin() * inv_scale,
                r.direction() * inv_scale,
                r.time()
            );

            if (!object->hit(scaled_ray, ray_t, rec))
                return false;

            // Transform hit point and normal back into world space
            rec.point = vec3(
                rec.point.x()*scale_factor.x(),
                rec.point.y()*scale_factor.y(),
                rec.point.z()*scale_factor.z()
            );

            vec3 normal = vec3(
                rec.normal.x() * inv_scale.x(),
                rec.normal.y() * inv_scale.y(),
                rec.normal.z() * inv_scale.z()
            );

            rec.normal = normal.normalize();

            return true;
        }

        aabb bounding_box() const override {
            return bbox;
        }

    private:
        /**
         * The object this scaling affects
         */
        shared_ptr<collidable> object;

        /**
         * The factor to scale by
         */
        vec3 scale_factor;

        /**
         * The inverse factor used to scale by
         */
        vec3 inv_scale;

        /**
         * The updated bounding box of the scaled object
         */
        aabb bbox;
};

#endif