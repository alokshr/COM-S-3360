#include <time.h>

#include "../renderlib.h"
#include "../camera.h"
#include "../collidable.h"
#include "../collidable_list.h"
#include "../sphere.h"

int main(int argc, char const *argv[])
{
    collidable_list world;

    // world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    // world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    // auto material_left   = make_shared<dielectric>(1.00 / 1.33);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera_config config = {
        1080,           //  int image_width;
        720,            //  int image_height;
        90,             //  double vfov;
        vec3(-2, 2, 1), //  vec3 lookfrom;
        vec3(0, 0, -1), //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        10,             //  int samples_per_pixel;
        50,             //  int max_depth;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "camera_img.ppm");
}
