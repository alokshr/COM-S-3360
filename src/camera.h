#ifndef CAMERA_H
#define CAMERA_H

#include "image.h"
#include "collidable.h"
#include "material.h"

#define EPSILON 0.001

/**
 * A struct for configuring common camera settings
 */
struct camera_config {
    int image_width;
    int image_height;
    double focal_length;
    int samples_per_pixel;
    int max_depth;
    double gamma;
};

/**
 * A class used to represent a camera in 3D space
 */
class camera {
    public:
        /**
         * Constructs a default camera
         */
        camera():
            center(vec3()),
            image_width(480),
            image_height(480),
            focal_length(1),
            samples_per_pixel(0),
            max_depth(3),
            gamma(1) {
                init();
        };

        /**
         * Constructs a camera with given settings
         * @param center the center of the camera in 3D space
         * @param image_width the width of the rendered image
         * @param image_height the height of the rendered image
         * @param focal_length the distance from the center to the viewport
         */
        camera(
            vec3 center,
            camera_config config
        ):
            center(center),
            image_width(config.image_width),
            image_height(config.image_height),
            focal_length(config.focal_length),
            samples_per_pixel(config.samples_per_pixel),
            max_depth(config.max_depth),
            gamma(config.gamma) {
                init();
        }

        /**
         * Renders a collidable object and outputs the rendered image to given filename
         * @param world collidable or collidable_list to render
         * @param filename name of file to save rendered image to
         */
        void render(const collidable& world, const char* filename) {
            const int progress_timer_max = image_height*image_width/20;
            int progress_timer = 10;
            bool anti_alias = samples_per_pixel > 0;
            for (int y = 0; y < image_height; y++) {
                for (int x = 0; x < image_width; x++) {
                    if (progress_timer == 0) {
                        print_progress(100.0*(y*image_width+x)/(image_width*image_height));
                        progress_timer = progress_timer_max;
                    }
                    progress_timer--;

                    color pixel_color = color();
                    if (anti_alias) {
                        for (int sample = 0; sample < samples_per_pixel; sample++) {
                            ray r = get_ray(x, y, sample_square());
                            pixel_color += ray_color(r, world, max_depth);
                        }
                        pixel_color = linear_to_gamma(pixel_color/samples_per_pixel);
                        img[y][x] = pixel_color;
                    } else {
                        ray r = get_ray(x, y, vec3());
                        pixel_color = linear_to_gamma(ray_color(r, world, max_depth));
                        img[y][x] = pixel_color;
                    }
                    // std::cout << pixel_color << std::endl;

                }
            }

            output_ppm_image(img, filename);
        }

    private:
        /**
         * The center of the camera in 3D space
         */
        vec3 center;

        /**
         * The number of samples used for anti-aliasing
         * 
         * If set to zero, no anti-aliasing is used
         */
        int samples_per_pixel;

        /**
         * The maximum number of ray bounces allowed
         */
        int max_depth;

        /**
         * The gamma value used for light correction
         */
        double gamma;

        /**
         * The width of the final rendered images
         */
        int image_width;

        /**
         * The height of the final rendered images
         */
        int image_height;

        /**
         * The distance from the camera's center to the viewport
         */
        double focal_length;

        /**
         * The width of the camera's viewport
         */
        double viewport_width;

        /**
         * The height of the camera's viewport
         */
        double viewport_height;

        /**
         * The location of pixel (0,0)
         */
        vec3 pixel00_loc;

        /**
         * The offset to the pixel to the right
         */
        vec3 pixel_delta_u;

        /**
         * The offset to the pixel below
         */
        vec3 pixel_delta_v;

        /**
         * The image that is rendered by this camera
         */
        image img;

        void init() {
            center = vec3(0, 0, 0);

            // Determine viewport dimensions.
            double focal_length = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            vec3 viewport_upper_left =
                center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            img = image(image_height, std::vector<color>(image_width, color()));
        }

        ray get_ray(int x, int y, vec3 offset) const {
            vec3 pixel_sample = pixel00_loc
                + ((x + offset[0])*pixel_delta_u)
                + ((y + offset[1])*pixel_delta_v);

            return ray(center, pixel_sample-center);
        }

        vec3 sample_square() const {
            return vec3(
                random() - 0.5,
                random() - 0.5,
                0
            );
        }

        inline double linear_to_gamma(double value) const {
            return (value > 0) ? pow(value, 1.0/gamma) : 0;
        }

        inline color linear_to_gamma(color c) const {
            return color(
                linear_to_gamma(c[0]),
                linear_to_gamma(c[1]),
                linear_to_gamma(c[2])
            );
        }

        color ray_color(const ray& r, const collidable& world, int depth) const {
            if (depth <= 0) {
                return color();
            }

            collision_hit rec;

            if (world.hit(r, interval(EPSILON, infinity), rec)) {
                ray scattered;
                color attenuation;

                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, world, depth-1);
                }
                
                return color(0, 0, 0);

                // Return normal as color
                // return 0.5 * (rec.normal + color(1,1,1));
            }

            vec3 unit_direction = r.direction().normalize();
            auto a = 0.5*(unit_direction.y() + 1.0);
            return lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), a);
        }

        void print_progress(float percentage) {
            std::clog << "\rRendering: " << percentage << std::flush;
        }
};

#endif