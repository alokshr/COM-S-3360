#ifndef COLLIDABLE_LIST_H
#define COLLIDABLE_LIST_H

#include "renderlib.h"
#include "collidable.h"
#include "aabb.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

/**
 * A list to allow collision checks against multiple collidables
 */
class collidable_list : public collidable {
    public:

        /**
         * A vector storing pointers to each collidable
         */
        std::vector<shared_ptr<collidable>> objects;

        /**
         * Creates an empty collidable_list
         */
        collidable_list() {};

        /**
         * Creates a collidable_list with the given object in it
         * @param object pointer to a collidable
         */
        collidable_list(shared_ptr<collidable> object) { add(object); };

        /**
         * Adds a collidable to this list
         * @param object pointer to a collidable
         */
        void add(shared_ptr<collidable> object) {
            // Add object and expand bbox to enclose it
            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }

        /**
         * Clears this list of all collidables
         */
        void clear() {
            objects.clear();
        }

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            collision_hit temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

        aabb bounding_box() const override { return bbox; }
        
        double pdf_value(const vec3& origin, const vec3& direction) const override {
            if (objects.size() == 0) return 0.0;
            double weight = 1.0 / objects.size();
            double sum = 0.0;

            for (const auto& object : objects)
                sum += weight * object->pdf_value(origin, direction);

            return sum;
        }

        vec3 random(const vec3& origin) const override {
            if (objects.size() == 0) return vec3(1, 0, 0);
            auto int_size = int(objects.size());
            return objects[random_int(0, int_size-1)]->random(origin);
        }
    private:
        /**
         * The bounding box that surrounds all collidables in this list
         */
        aabb bbox;
};
#endif