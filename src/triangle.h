#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "collidable.h"
#include "collidable_list.h"
#include "material.h"

using std::shared_ptr;

class triangle : public collidable {
    public:
        /**
         * Creates a non-normally interpolated triangle given three vertices and a material
         * @param a starting vertex
         * @param b next vertex
         * @param c ending vertex
         * @param mat material of triangle
         */
        triangle(vec3 a, vec3 b, vec3 c, shared_ptr<material> mat) : a(a), b(b), c(c), mat(mat), interpolated(false) {
            aabb ab = aabb(a, b);
            aabb ac = aabb(a, c);
            bbox = aabb(ab, ac);

            normal = vec3::cross(b-a, c-a).normalize();
        }

        /**
         * Creates a normally interpolated triangle given three vertices, three vertex normals, and a material
         * @param a starting vertex
         * @param b next vertex
         * @param c ending vertex
         * @param an starting vertex normal
         * @param bn next vertex normal
         * @param cn ending vertex normal
         * @param mat material of triangle
         */
        triangle(
            vec3 a, vec3 b, vec3 c,
            vec3 an, vec3 bn, vec3 cn,
            shared_ptr<material> mat) :
            a(a), b(b), c(c),
            an(an), bn(bn), cn(cn),
            mat(mat), interpolated(true) {
            
            aabb ab = aabb(a, b);
            aabb ac = aabb(a, c);
            bbox = aabb(ab, ac);
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const {
            // Fast, Minimum Storage Ray/Triangle Intersection implementation

            // Get edges
            vec3 e1 = b - a;
            vec3 e2 = c - a;

            vec3 P = vec3::cross(r.direction(), e2);
            
            // Calculate determinant
            double det = vec3::dot(e1, P);

            // Ray is parallel to the plane, so return false
            if (det > -1e-6 && det < 1e-6) {
                return false;
            }
            
            // Check u and v coords for collision
            vec3 T = r.origin() - a;
            double u = vec3::dot(T, P) / det;
            if (u < 0 || u > 1) {
                return false;
            }

            const vec3 Q = vec3::cross(T, e1);
            double v = vec3::dot(r.direction(), Q) / det;
            if (v < 0 || u + v > 1) {
                return false;
            }

            // Ray hit, so calculate true t, u, and v values
            double t = vec3::dot(e2, Q) / det;

            // t value out of ray range, no hit
            if (!ray_t.contains(t)) { 
                return false;
            }

            // Updating collision info
            rec.mat = mat;
            rec.t = t;
            rec.u = u;
            rec.v = v;

            if (!interpolated) {
                rec.point = r.at(t);
                rec.set_face_normal(r, normal);
            } else {
                rec.point = u*a + v*b + (1-u-v)*c;
                rec.set_face_normal(r, (u*an + v*bn + (1-u-v)*cn).normalize());
            }
            
            return true;            
        }
        aabb bounding_box () const override { return bbox; }
    private:
        /**
         * The vertices of this triangle in 3D space
         */
        vec3 a, b, c;

        /**
         * The normals of each vertex if given;
         */
        vec3 an, bn, cn;
        
        /**
         * The face normal of this triangle
         */
        vec3 normal;

        /**
         * Determines whether normal interpolation should be applied to this triangle or not
         */
        bool interpolated;

        /**
         * The bounding box of this triangle
         */
        aabb bbox;

        /**
         * The material of this triangle
         */
        shared_ptr<material> mat;
};

class triangle_mesh : public collidable {
    public:

    private:
        shared_ptr<collidable_list> triangles;

};

#endif