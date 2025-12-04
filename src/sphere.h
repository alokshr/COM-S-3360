#ifndef SPHERE_H
#define SPHERE_H

#include "renderlib.h"
#include "collidable.h"

class sphere : public collidable {
    public:
        sphere(
            const vec3& center,
            double radius,
            shared_ptr<material> mat
        ):
            center(center),
            radius(std::fmax(0,radius)),
            mat(mat) {
            vec3 rad = vec3(radius, radius, radius);
            bbox = aabb(center-rad, center+rad);
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const {
            vec3 oc = center - r.origin();
            double a = r.direction().sqmag();
            double h = vec3::dot(r.direction(), oc);
            double c = oc.sqmag() - radius*radius;
            double discriminant = h*h - a*c;

            double sqrtd = std::sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            double root = (h - sqrtd) / a;

            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;

                if (!ray_t.surrounds(root))
                    return false;
            }
            
            rec.t = root;
            rec.point = r.at(rec.t);
            vec3 outward_normal = (rec.point - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        vec3 center;
        double radius;
        std::shared_ptr<material> mat;
        aabb bbox;
};
#endif