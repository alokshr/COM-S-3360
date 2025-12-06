#ifndef QUAD_H
#define QUAD_H

#include "collidable.h"

using std::shared_ptr;

class quad : public collidable{
    public:
        quad(const vec3& q, const vec3& u, const vec3& v, shared_ptr<material> mat) :
            q(q), u(u), v(v), mat(mat) {
            
            vec3 n = vec3::cross(u, v);
            normal = n.normalize();
            d = vec3::dot(normal, q);
            w = n / n.sqmag();

            set_bounding_box();
        }

        virtual void set_bounding_box() {
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

            // Use planar coordinates to find if the hit point lies inside the quad
            vec3 intersection = r.at(t);
            vec3 hit_vec = intersection - q;
            double alpha = vec3::dot(w, vec3::cross(hit_vec, v));
            double beta = vec3::dot(w, vec3::cross(u, hit_vec));

            if (!is_interior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.point = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);
            
            return true;
        }

        /**
         * Returns whether planar coordinates (a, b) are inside of this quad and updates uv coords
         * @param a planar coord of u basis vector
         * @param b planar coord of v basis vector
         * @param rec hit info to update if the hit is inside this quad
         * @return true if hit, false if not; updates rec if hit
         */
        virtual bool is_interior(double a, double b, collision_hit& rec) const {
            const interval unit_interval = interval(0, 1);
            
            if (!unit_interval.contains(a) || !unit_interval.contains(b)) return false;

            rec.u = a;
            rec.v = b;
            return true;
        }

        aabb bounding_box() const override { return bbox; }
    private:
        vec3 q;
        vec3 u, v;
        vec3 w;
        shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;
        double d;
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