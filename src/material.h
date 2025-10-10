#ifndef MATERIAL_H
#define MATERIAL_H

#include "collidable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered
        ) const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo): albedo(albedo) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
        const override {
            vec3 scatter_dir = rec.normal + randvec3().normalize();

            if (scatter_dir.near_zero()) {
                scatter_dir = rec.normal;
            }

            scattered = ray(rec.point, scatter_dir);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double fuzz): albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflection = reflect(r_in.direction(), rec.normal);
            reflection = reflection.normalize() + (fuzz * randvec3().normalize());
            
            scattered = ray(rec.point, reflection);
            attenuation = albedo;
            return (vec3::dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};
#endif