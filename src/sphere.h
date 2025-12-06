#ifndef SPHERE_H
#define SPHERE_H

#include "renderlib.h"
#include "collidable.h"

/**
 * A class for rendering sphere primitives
 */
class sphere : public collidable {
    public:
        /**
         * Creates a sphere with a given center, radius, and material
         * @param center center of sphere
         * @param radius radius of sphere
         * @param mat material of sphere
         */
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
            // Solve quadratic equation for t
            vec3 oc = center - r.origin();
            double a = r.direction().sqmag();
            double h = vec3::dot(r.direction(), oc);
            double c = oc.sqmag() - radius*radius;
            double discriminant = h*h - a*c;

            double sqrtd = std::sqrt(discriminant);

            // Find if the calculated roots are in range of this ray
            double t = (h - sqrtd) / a;

            if (!ray_t.surrounds(t)) {
                t = (h + sqrtd) / a;

                if (!ray_t.surrounds(t)) return false;
            }
            
            // Root is in range, so we update collision info
            rec.t = t;
            rec.point = r.at(rec.t);
            vec3 outward_normal = (rec.point - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        /**
         * The center of this sphere
         */
        vec3 center;

        /**
         * The radius of this sphere
         */
        double radius;

        /**
         * The material of this sphere
         */
        std::shared_ptr<material> mat;

        /**
         * The bounding box of this sphere
         */
        aabb bbox;

        /**
         * Updates uv-coords to be coords of a point on a unit sphere where  
         * 
         * u: returned value [0,1] of angle around the Y axis from X=-1.
         * v: returned value [0,1] of angle from Y=-1 to Y=+1.
         * @param p point on unit sphere
         * @param u u value to update
         * @param v v value to update
         */
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