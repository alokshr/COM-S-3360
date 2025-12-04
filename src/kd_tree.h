#ifndef KD_TREE_H
#define KD_TREE_H

#include "aabb.h"
#include "collidable_list.h"

#include <vector>
#include <algorithm>

/**
 * An implementation of a 3-dimensional kd tree
 */
class kd_tree : public collidable {
    public:
        kd_tree(collidable_list list) : kd_tree(list.objects, 0, list.objects.size()) {}

        kd_tree(std::vector<shared_ptr<collidable>>& objects, size_t start, size_t end) {
            int axis = random_int(0, 2);
            auto comp = (axis == 1) ? aabb_comp_y : (axis == 2) ? aabb_comp_z : aabb_comp_x;

            size_t range = end - start;

            if (range == 1) {
                left = right = objects[start];
            } else if (range == 2) {
                left = objects[start];
                right = objects[start+1];
            } else {
                std::sort(std::begin(objects) + start, std::begin(objects) + end, comp);

                int mid = start + range/2;

                left = make_shared<kd_tree>(objects, start, mid);
                right = make_shared<kd_tree>(objects, mid, end);
            }

            bbox = aabb(left->bounding_box(), right->bounding_box());
        }
        
        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            if (!bbox.hit(r, ray_t)) return false;

            // If we are hit, search deeper down the tree
            bool hit_left = left->hit(r, ray_t, rec);
            bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        shared_ptr<collidable> left;
        shared_ptr<collidable> right;
        aabb bbox;

        static bool aabb_comp(const shared_ptr<collidable> a, const shared_ptr<collidable> b, int axis_index) {
            interval a_axis_interval = a->bounding_box().axis_interval(axis_index);
            interval b_axis_interval = b->bounding_box().axis_interval(axis_index);
            return a_axis_interval.min < b_axis_interval.min;
        }

        static bool aabb_comp_x (const shared_ptr<collidable> a, const shared_ptr<collidable> b) {
            return aabb_comp(a, b, 0);
        }

        static bool aabb_comp_y (const shared_ptr<collidable> a, const shared_ptr<collidable> b) {
            return aabb_comp(a, b, 1);
        }

        static bool aabb_comp_z (const shared_ptr<collidable> a, const shared_ptr<collidable> b) {
            return aabb_comp(a, b, 2);
        }
};

#endif