#ifndef CAMERA_H
#define CAMERA_H

#include "thread_pool.h"
#include "image.h"
#include "collidable.h"
#include "material.h"
#include "mathutils.h"

#define EPSILON 0.001

/**
 * A struct for configuring common camera settings
 */
struct camera_config {
    int image_width = 480;
    int image_height = 480;
    double vfov = 90;
    vec3 lookfrom, lookat, up;
    int samples_per_batch = 8;
    int batches_per_pixel = 4;
    double max_tolerance = 0.05;
    int max_depth = 3;
    double defocus_angle = 0;
    double focus_dist = 10;
    double gamma = 1;
    color background = color(0.7, 0.8, 1.0);
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
            camera(camera_config()) {
                init();
        };

        /**
         * Constructs a camera with given settings
         * @param config configuration settings of the camera
         */
        camera(
            camera_config config
        ):
            lookfrom(config.lookfrom),
            lookat(config.lookat),
            up(config.up),
            image_width(config.image_width),
            image_height(config.image_height),
            vfov(config.vfov),
            batches_per_pixel(config.batches_per_pixel),
            samples_per_batch(config.samples_per_batch),
            max_tolerance(config.max_tolerance),
            max_depth(config.max_depth),
            defocus_angle(config.defocus_angle),
            focus_dist(config.focus_dist),
            gamma(config.gamma),
            background(config.background) {
                init();
        }

        /**
         * Renders a collidable object and outputs the rendered image to given filename
         * @param world collidable to render
         * @param filename name of file to save rendered image to
         * @param num_threads the number of threads to run to render the image,
         *                    values less than 1 default to a singly-threaded render
         */
        void render(const collidable& world, const std::string& filename, int num_threads = 1) {
            bool multithreaded = num_threads > 1;
            std::clog << "Rendering " << filename << " using " << (multithreaded ? num_threads : 1) << " thread" << (multithreaded ? "s:" : ":") << std::endl;
            const bool anti_alias = samples_per_batch > 0;
            print_progress(0);

            if (multithreaded) {
                // Distribute each pixel's render as a job for threads
                thread_pool pool = thread_pool(num_threads);

                for (int y = 0; y < image_height; y++) {
                    for (int x = 0; x < image_width; x++) {
                        pool.enqueue([this, x, y, &world, anti_alias]{
                            render_pixel(x, y, world, anti_alias);
                        });
                    }
                }
                
                // Show progress
                int progress = 0;
                while (pool.get_progress_percent() < 1) {
                    int current_progress = pool.get_progress_percent()*100;

                    if (current_progress != progress) {
                        progress = current_progress;
                        print_progress(progress);
                    } 
                }
            } else {
                int progress = 0;

                for (int y = 0; y < image_height; y++) {
                    for (int x = 0; x < image_width; x++) {
                        render_pixel(x, y, world, anti_alias);

                        // Show progress
                        int current_progress = 100*(y * image_width + x)/(image_width*image_height);

                        if (current_progress != progress) {
                            progress = current_progress;
                            print_progress(progress);
                        } 
                    }
                }
            }
            
            print_progress(100);
            std::clog << "\n";
            output_ppm_image(img, filename);
        }

    private:
        /**
         * The center or origin of the camera in 3D space
         */
        vec3 lookfrom;

        /**
         * The point where the camera's focal point lies
         */
        vec3 lookat;

        /**
         * The direction that the camera considered up
         */
        vec3 up;

        /**
         * The camera's basis vectors
         */
        vec3 i, j, k;

        /**
         * The number of samples to take per batch of adaptive sampling
         * 
         */
        int samples_per_batch;

        /**
         * The number of sample batches used for anti-aliasing
         * 
         * If set to zero, no anti-aliasing is used
         */
        int batches_per_pixel;

        /**
         * The number used to determine the level of convergence necessary for adaptive sampling
         */
        double max_tolerance;

        /**
         * The maximum number of ray bounces allowed
         */
        int max_depth;

        /**
         * The gamma value used for light correction
         */
        double gamma;

        /**
         * Max angle of rays casted from the camera's defocus disk
         */
        double defocus_angle;

        /**
         * The distance from the camera where all "incoming" light rays converge
         */
        double focus_dist;

        /**
         * The width of the final rendered images
         */
        int image_width;

        /**
         * The height of the final rendered images
         */
        int image_height;

        /**
         * The vertical degrees of vision
         */
        double vfov;

        /**
         * The background color that appears rays don't hit any objects 
         */
        color background;

        /**
         * The location of the viewport's (0,0) pixel
         */
        vec3 pixel00_loc;

        /**
         * The horizontal and vertical offset to the next pixel of the viewport
         */
        vec3 pixel_delta_u, pixel_delta_v;

        /**
         * The horizontal and vertical radius of the defocus disk
         */
        vec3 defocus_disk_u, defocus_disk_v;

        /**
         * The image that is rendered by this camera
         */
        image img;

        void init() {
            lookfrom;

            // Determine viewport dimensions.
            double theta = d2r(vfov);
            double h = tan(theta/2.0);
            double viewport_height = 2.0 * h * focus_dist;
            double viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            k = (lookfrom-lookat).normalize();
            i = vec3::cross(up, k).normalize();
            j = vec3::cross(k, i);

            vec3 viewport_u = viewport_width * i;
            vec3 viewport_v = -viewport_height * j;

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            vec3 viewport_upper_left =
                lookfrom - focus_dist*k - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            double defocus_radius = focus_dist * tan(d2r(defocus_angle/2.0));
            defocus_disk_u = i * defocus_radius;
            defocus_disk_v = j * defocus_radius;

            img = image(image_height, std::vector<color>(image_width, color()));
        }

        /**
         * Renders a single pixel on this camera's image
         * @param x image x coord
         * @param y image y coord
         * @param world collidable to render
         * @param anti_alias if true, use multiple randomly sampled rays, if false, use only one ray
         */
        void render_pixel(int x, int y, const collidable& world, bool anti_alias) {
            color pixel_color = color();
            if (anti_alias) {
                double s1 = 0;
                double s2 = 0;
                int n = 0;
                for (int batch = 0; batch < batches_per_pixel; batch++) {
                    for (int sample = 0; sample < samples_per_batch; sample++) {
                        ray r = get_ray(x, y, sample_square());
                        color c = ray_color(r, world, max_depth);
                        pixel_color += c;
                        s1 += illuminance(c);
                        s2 += std::pow(illuminance(c), 2);
                        n++;
                    }
                    // Last batch doesn't require these additional calculations,
                    // skip if the last batch was calculated
                    if (batch == batches_per_pixel - 1) break;

                    // Check if we have converged to a single value
                    double mu = s1/n;
                    double epsilon2 = (s2 - std::pow(s1, 2)/n)/(n-1);

                    // Construct a 95% confidence interval
                    double I = 1.96 * std::sqrt(epsilon2/n);
                    if (I <= max_tolerance*mu) break;
                }
                // pixel_color = lerp(color(), color(1, 1, 1), n/(batches_per_pixel*samples_per_batch));
                pixel_color = linear_to_gamma(pixel_color/n);
            } else {
                ray r = get_ray(x, y, vec3());
                pixel_color = linear_to_gamma(ray_color(r, world, max_depth));
            }
            img[y][x] = pixel_color;
        }

        /**
         * Returns a ray originated at the camera or defocus disk, pointing through the given pixel
         * @param x x coord of pixel
         * @param y y coord of pixel
         * @param offset offset from pixel coord  
         * @return ray from camera/defocus disk pointing through the pixel
         */
        ray get_ray(int x, int y, vec3 offset) const {
            vec3 pixel_sample = pixel00_loc
                + ((x + offset[0])*pixel_delta_u)
                + ((y + offset[1])*pixel_delta_v);

            vec3 ray_origin = (defocus_angle > 0) ? defocus_disk_sample() : lookfrom;

            return ray(ray_origin, pixel_sample-ray_origin, random_double());
        }

        vec3 sample_square() const {
            return vec3(
                random_double() - 0.5,
                random_double() - 0.5,
                0
            );
        }

        /**
         * Returns a uniformly sampled point inside of a unit circle
         * @return vec3 with the coords in the x and y components, z is 0  
         */
        vec3 rand_in_unit_circle() const {
            double theta = sqrt(random_double())*2*M_PI;
            return random_double()*vec3(cos(theta), sin(theta), 0);
        }

        /**
         * Returns a point sampled from this camera's defocus disk
         * @return point on defocus disk
         */
        vec3 defocus_disk_sample() const {
            vec3 p = rand_in_unit_circle();
            return lookfrom + p[0]*defocus_disk_u + p[1]*defocus_disk_v;
        }

        /**
         * Scales a scalar value to the camera's gamma scale
         */
        inline double linear_to_gamma(double value) const {
            return (value > 0) ? pow(value, 1.0/gamma) : 0;
        }

        /**
         * Scales a vec3 to the camera's gamma scale
         */
        inline color linear_to_gamma(color c) const {
            return color(
                linear_to_gamma(c[0]),
                linear_to_gamma(c[1]),
                linear_to_gamma(c[2])
            );
        }

        /**
         * Returns the final color of a ray after bouncing off of a collidable object
         * @param r ray to project
         * @param world object to collide with
         * @param depth number of bounces before stopping and returning white for the last color
         */
        color ray_color(const ray& r, const collidable& world, int depth) const {
            if (depth <= 0) {
                return color();
            }

            collision_hit rec;

            if (!world.hit(r, interval(EPSILON, infinity), rec)) {
                return background;
            }

            ray scattered;
            color attenuation;

            color emission = rec.mat->emit(rec.u, rec.v, rec.point);

            if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
                return emission;
            }
            
            color scatter = attenuation * ray_color(scattered, world, depth-1);

            return emission + scatter;

            // vec3 unit_direction = r.direction().normalize();
            // auto a = 0.5*(unit_direction.y() + 1.0);
            // return lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), a);
        }

        void print_progress(int progress) {
            std::clog << "\rRendering: [" << std::string(progress / 2, '#') << std::string(50 - progress / 2, '-') << "] " << progress << "%" << std::flush;
        }
};

#endif