#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "collidable.h"
#include "collidable_list.h"
#include "material.h"

using std::shared_ptr;

class triangle : public collidable {
    public:
        /**
         * Creates a triangle given three vertices, three vertex texture coodinates, three vertex normals, and a material
         * @param a starting vertex
         * @param b next vertex
         * @param c ending vertex
         * @param ta starting vertex texture coords
         * @param tb next vertex texture coords
         * @param tc ending vertex texture coords
         * @param na starting vertex normal
         * @param nb next vertex normal
         * @param nc ending vertex normal
         * @param mat material of triangle
         */
        triangle(
            vec3 a, vec3 b, vec3 c,
            vec3 ta, vec3 tb, vec3 tc,
            vec3 na, vec3 nb, vec3 nc,
            shared_ptr<material> mat) :
            a(a), b(b), c(c),
            ta(ta), tb(tb), tc(tc),
            na(na.normalize()), nb(nb.normalize()), nc(nc.normalize()),
            mat(mat), interpolated(true) {
            
            aabb ab = aabb(a, b);
            aabb ac = aabb(a, c);
            bbox = aabb(ab, ac);
        }

        /**
         * Creates a triangle given three vertices and a material
         * @param a starting vertex
         * @param b next vertex
         * @param c ending vertex
         * @param mat material of triangle
         */
        triangle(vec3 a, vec3 b, vec3 c, shared_ptr<material> mat) : a(a), b(b), c(c), mat(mat), interpolated(false) {
            aabb ab = aabb(a, b);
            aabb ac = aabb(a, c);
            bbox = aabb(ab, ac);

            ta = vec3(0, 0, 0);
            tb = vec3(1, 0, 0);
            tc = vec3(0, 1, 0);
            normal = vec3::cross(b-a, c-a).normalize();
            na = nb = nc = normal;
        }

        /**
         * Creates a triangle given three vertices, three vertex texture coordinates or three vertex normals, and a material
         * @param a starting vertex
         * @param b next vertex
         * @param c ending vertex
         * @param ea starting vertex texture coordinate or normal
         * @param eb next vertex texture coordinate or normal
         * @param ec ending vertex texture coordinate or normal
         * @param mat material of triangle
         * @param is_tex_coords true if ea, eb, ec represent vertex texture coordinates,
         *                      false if ea, eb, ec represent vertex normals
         */
        triangle(
            vec3 a, vec3 b, vec3 c,
            vec3 ea, vec3 eb, vec3 ec,
            shared_ptr<material> mat, bool is_tex_coords) : a(a), b(b), c(c), mat(mat) {
            
            if (is_tex_coords) {
                ta = ea;
                tb = eb;
                tc = ec;
                na = nb = nc = normal = vec3::cross(b-a, c-a).normalize();
                interpolated = false;
            } else {
                na = ea.normalize();
                nb = eb.normalize();
                nc = ec.normalize();
                ta = vec3(0, 0, 0);
                tb = vec3(1, 0, 0);
                tc = vec3(0, 1, 0);
                interpolated = true;
            }

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
            
            // Check barycentric coords (alpha, beta) for collision
            vec3 T = r.origin() - a;
            double alpha = vec3::dot(T, P) / det;
            if (alpha < 0 || alpha > 1) {
                return false;
            }

            const vec3 Q = vec3::cross(T, e1);
            double beta = vec3::dot(r.direction(), Q) / det;
            if (beta < 0 || alpha + beta > 1) {
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

            vec3 tex_coords = (1-alpha-beta)*ta + alpha*tb + beta*tc;

            rec.u = tex_coords[0];
            rec.v = tex_coords[1];

            rec.point = r.at(t);
            if (!interpolated) {
                rec.set_face_normal(r, normal);
            } else {
                rec.set_face_normal(r, ((1-alpha-beta)*na + alpha*nb + beta*nc).normalize());
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
         * The texture coords of each vertex, if given
         */
        vec3 ta, tb, tc;

        /**
         * The normals of each vertex, if given
         */
        vec3 na, nb, nc;
        
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