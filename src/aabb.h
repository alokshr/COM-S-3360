#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "ray.h"
#include "interval.h"

/**
 * A class for using axis-aligned bounding boxes
 */
class aabb {
    public:
        /**
         * The minimum and maximum range of the bounding box's vertices  
         */
        interval x, y, z;
        
        /**
         * Creates a zero-size bounding box
         */
        aabb() {}

        /**
         * Creates a bounding box given the lengths in each dimension
         * @param x interval of the box's x length
         * @param y interval of the box's y length
         * @param z interval of the box's z length
         */
        aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {
            pad_to_minimums();
        }

        /**
         * Creates a bounding box from two extremes
         * @param a corner of the bounding box
         * @param b corner of the bounding box
         */
        aabb(const vec3& a, const vec3& b) {
            //
            x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
        }

        /**
         * Creates a bounding box around two given bounding boxes
         * @param a bounding box to surround
         * @param b bounding box to surround
         */
        aabb(const aabb& box0, const aabb& box1) {
            x = interval(box0.x, box1.x);
            y = interval(box0.y, box1.y);
            z = interval(box0.z, box1.z);
        }

        /**
         * Returns the axis interval associated with its number
         * @param n number of axis
         * @return axis interval belonging to given number
         */
        const interval& axis_interval(int n) const {
            if (n == 1) return y;
            if (n == 2) return z;
            return x;
        }

        /**
         * Returns whether or not a ray at a given interval collides with this bounding box
         * @param r ray to check
         * @param ray_t interval of ray to check
         * @returns true if ray collides, false if ray doesn't collide
         */
        bool hit(const ray& r, interval ray_t) const {
            const vec3& ray_orig = r.origin();
            const vec3& ray_dir = r.direction();

            // For each axis, check if the ray lies with its interval
            for (int axis = 0; axis < 3; axis++) {
                const interval& ax = axis_interval(axis);

                double t0 = (ax.min - ray_orig[axis]) / ray_dir[axis];
                double t1 = (ax.max - ray_orig[axis]) / ray_dir[axis];

                if (t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min) return false;
            }
            return true;
        }  

        /**
         * Returns the index of the longest axis of the bounding box
         */
        int longest_axis() const {
            if (x.size() > y.size())
                return x.size() > z.size() ? 0 : 2;
            else
                return y.size() > z.size() ? 1 : 2;
        }

        static const aabb empty, universe;
        
    private:

        void pad_to_minimums() {
            const double delta = 0.0001;
            if (x.size() < delta) x = x.expand(delta);
            if (y.size() < delta) y = y.expand(delta);
            if (z.size() < delta) z = z.expand(delta);
        }
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif