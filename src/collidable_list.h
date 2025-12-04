#ifndef COLLIDABLE_LIST_H
#define COLLIDABLE_LIST_H

#include "renderlib.h"
#include "collidable.h"
#include "aabb.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class collidable_list : public collidable {
    public:
        std::vector<shared_ptr<collidable>> objects;

        collidable_list() {};
        collidable_list(shared_ptr<collidable> object) { add(object); };

        void add(shared_ptr<collidable> object) {
            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }

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
    private:
        aabb bbox;
};
#endif