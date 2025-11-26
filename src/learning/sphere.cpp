#include <time.h>

#include "../camera.h"
#include "../collidable_list.h"
#include "../sphere.h"

int main(int argc, char const *argv[])
{
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
    
    camera_config config_mattest = {
        1080,           //  int image_width;
        720,            //  int image_height;
        20,             //  double vfov;
        vec3(-2, 2, 1), //  vec3 lookfrom;
        vec3(0, 0, -1), //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        10,             //  int samples_per_pixel;
        50,             //  int max_depth;
        10,             //  double defocus_angle;
        3.4,            //  double defocus_dist;
        2               //  double gamma;
    };

    collidable_list world_mattest;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world_mattest.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world_mattest.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world_mattest.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world_mattest.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world_mattest.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    camera_config config_orbfield = {
        1200,           //  int image_width;
        675,            //  int image_height;
        20,             //  double vfov;
        vec3(13, 2, 3), //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        1,              //  int samples_per_pixel;
        50,             //  int max_depth;
        0.6,            //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    collidable_list world_orbfield;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world_orbfield.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random();
            vec3 center(a + 0.9*random(), 0.2, b + 0.9*random());

            if ((center - vec3(4, 0.2, 0)).mag() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randvec3() * randvec3();
                    sphere_material = make_shared<lambertian>(albedo);
                    world_orbfield.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = randvec3(0.5, 1);
                    auto fuzz = random(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world_orbfield.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world_orbfield.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world_orbfield.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world_orbfield.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world_orbfield.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));


    camera cam_mattest(config_mattest);
    camera cam_orbfield(config_orbfield);

    cam_mattest.render(world_mattest, "mattest.ppm", std::thread::hardware_concurrency() - 1);
    cam_orbfield.render(world_orbfield, "orbfield.ppm", std::thread::hardware_concurrency() - 1);
}
