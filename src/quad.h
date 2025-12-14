#ifndef QUAD_H
#define QUAD_H

#include "collidable.h"

using std::shared_ptr;

/**
 * A class for rendering quads primitives
 */
class quad : public collidable{
    public:
        /**
         * Creates a quad with an given origin, basis vectors, and material
         * @param q origin of quad
         * @param u basis vector of quad
         * @param v basis vector of quad
         * @param mat material of quad
         */
        quad(const vec3& q, const vec3& u, const vec3& v, shared_ptr<material> mat) :
            q(q), u(u), v(v), mat(mat) {
            
            vec3 n = vec3::cross(u, v);
            normal = n.normalize();
            d = vec3::dot(normal, q);
            w = n / n.sqmag();

            area = n.mag();

            // Set bounding box based on diagonals
            aabb diag1 = aabb(q, q + u + v);
            aabb diag2 = aabb(q + u, q + v);
            bbox = aabb(diag1, diag2);
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            double den = vec3::dot(normal, r.direction());

            // Ray is parallel to the plane, so return false
            if (std::fabs(den) < 1e-8) return false;

            // t value is outside of our range, so return false
            double t = (d - vec3::dot(normal, r.origin()))/den;
            if (!ray_t.contains(t)) return false;

            // Use quad coordinates to find if the hit point lies inside the quad
            vec3 intersection = r.at(t);
            vec3 hit_vec = intersection - q;
            double alpha = vec3::dot(w, vec3::cross(hit_vec, v));
            double beta = vec3::dot(w, vec3::cross(u, hit_vec));

            if (!is_interior(alpha, beta, rec))
                return false;

            // Hit point is inside quad, update collision info
            rec.t = t;
            rec.point = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);
            
            return true;
        }

        /**
         * Returns whether quad coordinates (a, b) are inside of this quad and updates uv coords
         * @param a coord of u basis vector
         * @param b coord of v basis vector
         * @param rec collision info to update if the hit is inside this quad
         * @return true if hit, false if not; updates collision info if hit
         */
        virtual bool is_interior(double a, double b, collision_hit& rec) const {
            const interval unit_interval = interval(0, 1);
            
            // If our quad coords aren't between 0 and 1, they aren't inside the quad
            if (!unit_interval.contains(a) || !unit_interval.contains(b)) return false;

            rec.u = a;
            rec.v = b;
            return true;
        }

        aabb bounding_box() const override { return bbox; }

        double pdf_value(const vec3& origin, const vec3& direction) const override {
            // Ensure that incoming ray is sampling this quad
            collision_hit rec;
            if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
                return 0;

            // Get pdf of the given direction 
            auto distance_squared = rec.t * rec.t * direction.sqmag();
            auto cosine = std::fabs(vec3::dot(direction, rec.normal) / direction.mag());

            return distance_squared / (cosine * area);
        }

        vec3 random(const vec3& origin) const override {
            vec3 point_on_quad = random_double()*u + random_double()*v;
            return point_on_quad - origin;
        }
    private:
        /**
         * The origin point of this quad
         */
        vec3 q;

        /**
         * The basis vectors of this quad
         */
        vec3 u, v;
        
        /**
         * Constant for calculating quad coordinates
         */
        vec3 w;
        
        /**
         * The material of this quad
         */
        shared_ptr<material> mat;

        /**
         * The bounding box of this quad
         */
        aabb bbox;

        /**
         * The normal vector of this quad
         */
        vec3 normal;

        /**
         * Constant for calculating the t value of a ray collision
         */
        double d;

        /**
         * The surface area of this quad
         */
        double area;
};

inline shared_ptr<collidable_list> box(const vec3& a, const vec3& b, shared_ptr<material> mat)
{
    auto sides = make_shared<collidable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    vec3 min = vec3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    vec3 max = vec3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    vec3 x = vec3(max.x() - min.x(), 0, 0);
    vec3 y = vec3(0, max.y() - min.y(), 0);
    vec3 z = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(vec3(min.x(), min.y(), max.z()),  x,  y, mat)); // front
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -z,  y, mat)); // right
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -x,  y, mat)); // back
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()),  z,  y, mat)); // left
    sides->add(make_shared<quad>(vec3(min.x(), max.y(), max.z()),  x, -z, mat)); // top
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()),  x,  z, mat)); // bottom

    return sides;
}

#endif