#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "perlin.h"

#include <memory>

using std::make_shared;
using std::shared_ptr;

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const vec3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const color& albedo) : albedo(albedo) {}
        solid_color(double r, double g, double b) : albedo(color(r, g, b)) {}

        color value(double u, double v, const vec3& p) const override {
            return albedo;
        }

    private:
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

class checker_texture : public texture {
    public:
        checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

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
        double inv_scale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

class noise_texture : public texture {
    public:
        noise_texture(double scale) : scale(scale) {}
    
        color value(double u, double v, const vec3& p) const override {
            return color(1,1,1) * 0.5 * (1.0 + generator.noise(scale * p));
        }

    private:
        perlin generator;
        double scale;
};

#endif