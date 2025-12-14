#ifndef MATERIAL_H
#define MATERIAL_H

#include "collidable.h"
#include "texture.h"

/**
 * A class for representing materials
 */
class material {
    public:
        virtual ~material() = default;

        /**
         * Returns the color emitted by this material
         * @param u u coordinate
         * @param v v coordinate
         * @param p point in 3D space
         * @return emitted color
         */
        virtual color emit(double u, double v, const vec3& p) const {
            return color(0, 0, 0);
        }

        /**
         * Returns true if this ray scatters and a scattered ray 
         * @param r_in incoming ray to scatter
         * @param rec collision info of ray
         * @param attenuation current color collected
         * @param scattered outgoing ray to return
         * @return true if material scatters, false if material does not scatter
         */
        virtual bool scatter(
            const ray& r_in,
            const collision_hit& rec,
            color& attenuation, ray& scattered)
        const {
            return false;
        }

        /**
         * Returns the scattering pdf associated with this material
         * @param r_in incoming ray to scatter
         * @param rec collision info of ray
         * @param scattered outgoing scattered ray
         * @return pdf of material
         */
        virtual double scattering_pdf(
            const ray& r_in,
            const collision_hit& rec,
            const ray& scattered)
        const {
            return 0;
        }
};

/**
 * A class for creating diffuse materials with no variation based on viewing angle
 */
class lambertian : public material {
    public:
        /**
         * Creates a lambertian material with a solid color
         * @param albedo solid color
         */
        lambertian(const color& albedo): tex(make_shared<solid_color>(albedo)) {}

        /**
         * Creates a lambertian material with a given texture
         * @param tex pointer to texture
         */
        lambertian(shared_ptr<texture> tex): tex(tex) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
        const override {
            vec3 scatter_dir = rec.normal + randvec3().normalize();

            if (scatter_dir.near_zero()) {
                scatter_dir = rec.normal;
            }

            scattered = ray(rec.point, scatter_dir, r_in.time());
            attenuation = tex->value(rec.u, rec.v, rec.point);
            return true;
        }

        double scattering_pdf(
            const ray& r_in,
            const collision_hit& rec,
            const ray& scattered)
        const override {
            double cos_theta = vec3::dot(rec.normal, scattered.direction().normalize());
            return cos_theta < 0 ? 0 : cos_theta/M_PI; 
        }

    private:
        /**
         * Texture of this material
         */
        shared_ptr<texture> tex;
};

/**
 * A class for creating specular materials that reflects light like a mirror
 */
class metal : public material {
    public:
        /**
         * Creates a metal material with a solid color and level of fuzzy reflection
         * @param albedo solid color
         * @param fuzz percentage of fuzzy reflection, 0 for no fuzz, 1 for full fuzz
        */
        metal(const color& albedo, double fuzz): albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflection = reflect(r_in.direction(), rec.normal);
            reflection = reflection.normalize() + (fuzz * randvec3().normalize());
            
            scattered = ray(rec.point, reflection, r_in.time());
            attenuation = albedo;
            return (vec3::dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        /**
         * Color of this material
         */
        color albedo;

        /**
         * Percentage of fuzzy reflection, 0 for no fuzz, 1 for full fuzz
         */
        double fuzz;
};

/**
 * A class for making dielectric materials that allows light to reflect off and refract/bend through
 */
class dielectric : public material {
    public:
        /**
         * Creates a dielectric material with a given refractive index
         * @param refraction_index refractive index
         */
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = r_in.direction().normalize();
            double cos_theta = std::fmin(vec3::dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, ri);

            scattered = ray(rec.point, direction, r_in.time());
            return true;
        }

    private:
        /**
         * Refractive index of the material in a vacuum
        */
        double refraction_index;

        /**
         * Returns the reflectance probability using Schlick's approximation:
         * r(theta) = r0 + (1-r0)(1-cos(theta))^5
         * @param cosine calculated value of cosine
         * @param refraction_index refractive index of material
         * @return probability of reflecting
         */ 
        static double reflectance(double cosine, double refraction_index) {
            double r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1 - cosine),5);
        }
};

/**
 * A class for making emissive materials that emits light evenly
 */
class diffuse_light : public material {
    public:
        /**
         * Creates a diffuse_light material with a solid color
         * @param emit solid color
         */
        diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

        /**
         * Creates a diffuse_light material with a given texture
         * @param tex pointer to texture
         */
        diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
    
        color emit(double u, double v, const vec3& p) const override {
            return tex->value(u, v, p);
        }    
    private:
        /**
         * Texture of this material
         */
        shared_ptr<texture> tex;
};

/**
 * A class for uniformly scattering inside a volume in all directions
 */
class isotropic : public material {
    public:
        /**
         * Creates an isotropic material with a solid color used for volume scattering
         * @param albedo solid color of material
         */
        isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
        
        /**
         * Creates an isotropic material with a given texture used for volume scattering
         * @param tex texture of material
         */
        isotropic(shared_ptr<texture> tex) : tex(tex) {}

        bool scatter(const ray& r_in, const collision_hit& rec, color& attenuation, ray& scattered)
        const override {
            scattered = ray(rec.point, randvec3().normalize(), r_in.time());
            attenuation = tex->value(rec.u, rec.v, rec.point);
            return true;
        }

    private:
        shared_ptr<texture> tex;
};

#endif