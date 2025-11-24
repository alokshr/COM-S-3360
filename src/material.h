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

class dielectric : public material {
  public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        vec3 unit_direction = r_in.direction().normalize();
        double cos_theta = std::fmin(vec3::dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.point, direction);
        return true;
    }

  private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    // Calculates reflectance using Schlick's approximation.
    static double reflectance(double cosine, double refraction_index) {
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};
#endif