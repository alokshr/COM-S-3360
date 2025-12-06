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
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        vec3 center;
        double radius;
        std::shared_ptr<material> mat;
        aabb bbox;

         static void get_sphere_uv(const vec3& p, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = std::acos(-p.y());
            auto phi = std::atan2(-p.z(), p.x()) + M_PI;

            u = phi / (2*M_PI);
            v = theta / M_PI;
        }
};
#endif