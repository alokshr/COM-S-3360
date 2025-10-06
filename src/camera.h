#ifndef CAMERA_H
#define CAMERA_H

#include "renderlib.h"
#include "image.h"
#include "collision.h"
#include "collidable.h"

class camera {
    public:

        /**
         * Constructs a default camera
         */
        camera():
            center(vec3()),
            image_width(480),
            image_height(480),
            focal_length(1) {
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
            int image_width,
            int image_height,
            double focal_length
        ):
            center(center),
            image_width(image_width),
            image_height(image_height),
            focal_length(focal_length) {
                init();
        }

        /**
         * Renders a collidable object and outputs the rendered image to given filename
         * @param world collidable or collidable_list to render
         * @param filename name of file to save rendered image to
         */
        void render(const collidable& world, const char* filename) {
            for (int y = 0; y < image_height; y++) {
                for (int x = 0; x < image_width; x++) {
                    vec3 pixel_center = pixel00_loc + (x*pixel_delta_u) + (y*pixel_delta_v);
                    
                    ray r(center, pixel_center - center);

                    color pixel_color = ray_color(r, world);

                    img[y][x] = pixel_color;
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

        color ray_color(const ray& r, const collidable& world) const {
            collision_hit rec;

            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
            }

            vec3 unit_direction = r.direction().normalize();
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};

#endif