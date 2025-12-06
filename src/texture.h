#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "perlin.h"

#include <memory>

using std::make_shared;
using std::shared_ptr;

/**
 * A virtual class to represent textures
 */
class texture {
    public:
        virtual ~texture() = default;

        /**
         * Returns the color of this texture at the given uv-coords and point in 3D space
         * @param u u coord of texture to sample
         * @param v v coord of texture to sample
         * @param p point in 3D space
         * @return color of texture
         */
        virtual color value(double u, double v, const vec3& p) const = 0;
};

/**
 * A class for creating solid color textures
 */
class solid_color : public texture {
    public:
        /**
         * Creates a solid color texture
         * @param albedo solid color of texture
         */
        solid_color(const color& albedo) : albedo(albedo) {}

        color value(double u, double v, const vec3& p) const override {
            return albedo;
        }

    private:
        /**
         * The color of this texture;
         */
        color albedo;
};

// class linear_gradient_color : public texture {
//     public:
//         linear_gradient_color(const color& start, const color& end) : start(start), end(end) {}

//         color value(double u, double v, const vec3& p) const override {
//             return lerp(start, end, u);
//         }

//     private:
//         color start;
//         color end;
// };

/**
 * A class for creating checkboard pattern textures
 */
class checker_texture : public texture {
    public:
        /**
         * Creates a checkboard texture with a given square scale and textures
         * @param scale scale of checkboard squares
         * @param even texture of one set of checkboard squares
         * @param odd texture of other set of checkboard squares
         */
        checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

        /**
         * Creates a checkboard texture with a given square scale and solid colors
         * @param scale scale of checkboard squares
         * @param even color of one set of checkboard squares
         * @param odd color of other set of checkboard squares
         */
        checker_texture(double scale, const color& c1, const color& c2)
            : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

        color value(double u, double v, const vec3& p) const override {
            auto xInteger = int(std::floor(inv_scale * p.x()));
            auto yInteger = int(std::floor(inv_scale * p.y()));
            auto zInteger = int(std::floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        /**
         * The scaling factor of the checkboard squares
         */
        double inv_scale;

        /**
         * The texture for the even squares of the checkboard
         */
        shared_ptr<texture> even;

        /**
         * The texture for the odd squares of the checkboard
         */
        shared_ptr<texture> odd;
};

/**
 * A class for creating Perlin noise textures
 */
class noise_texture : public texture {
    public:
        /**
         * Creates a Perlin noise texture with a given scale
         * @param scale scale of noise
         */
        noise_texture(double scale) : scale(scale) {}
    
        color value(double u, double v, const vec3& p) const override {
            return color(1,1,1) * 0.5 * (1.0 + generator.noise(scale * p));
        }

    private:
        /**
         * The generator of this texture's Perlin noise
         */
        perlin generator;

        /**
         * The scaling factor of the Perlin noise
         */
        double scale;
};

#endif