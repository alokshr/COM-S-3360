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
            center(ray(center, vec3())),
            radius(std::fmax(0,radius)),
            mat(mat) {
            vec3 rad = vec3(radius, radius, radius);
            bbox = aabb(center-rad, center+rad);
        }

        /**
         * Creates a moving sphere with a radius and material given two centers to move between
         * @param center1 starting center of sphere
         * @param center2 ending center of sphere
         * @param radius radius of sphere
         * @param mat material of sphere
         */
        sphere(
            const vec3& center1,
            const vec3& center2,
            double radius,
            shared_ptr<material> mat
        ):
            center(ray(center1, center2-center1)),
            radius(std::fmax(0,radius)),
            mat(mat) {
                
            vec3 rad = vec3(radius, radius, radius);
            aabb bbox1 = aabb(center1-rad, center1+rad);
            aabb bbox2 = aabb(center2-rad, center2+rad);
            bbox = aabb(bbox1, bbox2);
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const {
            // Solve quadratic equation for t
            vec3 current_center = center.at(r.time());
            vec3 oc = current_center - r.origin();
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
            vec3 outward_normal = (rec.point - current_center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

        double pdf_value(const vec3& origin, const vec3& direction) const override {
            // This method only works for stationary spheres.

            collision_hit rec;
            if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
                return 0;

            auto dist_squared = (center.at(0) - origin).sqmag();
            auto cos_theta_max = std::sqrt(1 - radius*radius/dist_squared);
            auto solid_angle = 2*M_PI*(1-cos_theta_max);

            return  1 / solid_angle;
        }

        vec3 random(const vec3& origin) const override {
            vec3 direction = center.at(0) - origin;
            double distance_squared = direction.sqmag();
            onb ijk(direction);
            return ijk.transform(random_to_sphere(radius, distance_squared));
        }
    private:
        /**
         * The center of this sphere
         */
        ray center;

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

        /**
         * Returns a uniformly distributed random direction to a sphere from a point on the z axis
         * @param radius radius of sphere
         * @param distance_squared distance squared from sphere
         * @param random direction to sphere
         */
        static vec3 random_to_sphere(double radius, double distance_squared) {
            double r1 = random_double();
            double r2 = random_double();
            double z = 1 + r2*(std::sqrt(1-radius*radius/distance_squared) - 1);
            
            double phi = 2*M_PI*r1;
            double x = std::cos(phi) * std::sqrt(1-z*z);
            double y = std::sin(phi) * std::sqrt(1-z*z);

            return vec3(x, y, z);
        }
};
#endif