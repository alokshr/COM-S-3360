#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "collidable.h"
#include "material.h"
#include "texture.h"

/**
 * A class for rendering volumes with constant density
 */
class constant_medium : public collidable {
    public:
        /**
         * Creates a volume with a given boundary, density, and texture
         * @param boundary collidable object to fill with volume
         * @param density density of volume
         * @param tex texture of volume
         */
        constant_medium(shared_ptr<collidable> boundary, double density, shared_ptr<texture> tex)
        : boundary(boundary), neg_inv_density(-1/density),
            phase_function(make_shared<isotropic>(tex))
        {}

        /**
         * Creates a volume with a given boundary, density, and solid color
         * @param boundary collidable object to fill with volume
         * @param density density of volume
         * @param albedo solid color of volume
         */
        constant_medium(shared_ptr<collidable> boundary, double density, const color& albedo)
        : boundary(boundary), neg_inv_density(-1/density),
            phase_function(make_shared<isotropic>(albedo))
        {}

        bool hit(const ray& r, interval ray_t, collision_hit& rec) const override {
            collision_hit rec1, rec2;

            // Get where the ray goes into the boundary, ray doesn't hit boundary means ray can't scatter
            if (!boundary->hit(r, interval::universe, rec1))
                return false;

            // Get where the ray leaves the boundary, ray doesn't hit boundary means ray can't scatter
            if (!boundary->hit(r, interval(rec1.t+0.0001, infinity), rec2))
                return false;

            // Clamp ingoing and outgoing t values to ray's t values
            rec1.t = std::max(rec1.t, ray_t.min);
            rec2.t = std::min(rec2.t, ray_t.max);

            // Ray doesn't lie within ray_t interval, ray doesn't hit
            if (rec1.t >= rec2.t)
                return false;

            rec1.t = std::max(rec1.t, 0.0);

            // Get distance travelled in volume
            double ray_length = r.direction().mag();
            double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;

            // Get a scattering distance
            double hit_distance = neg_inv_density * std::log(random_double());

            // If scattering distance isn't in the boundary, we don't scatter
            if (hit_distance > distance_inside_boundary)
                return false;

            // Scatter point lands inside boundary, update ray collision info with new scatter point
            rec.t = rec1.t + hit_distance / ray_length;
            rec.point = r.at(rec.t);

            // Normals don't matter for volume collisions/scattering
            rec.normal = vec3(1,0,0);
            rec.front_face = true;

            // Scatter according to given function
            rec.mat = phase_function;

            return true;
        }

        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        /**
         * The collidable object this volume fills
         */
        shared_ptr<collidable> boundary;

        /**
         * The inverse density of this volume
         */
        double neg_inv_density;

        /**
         * The material that determines the scattering inside the volume
         */
        shared_ptr<material> phase_function;
};

#endif