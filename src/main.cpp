#include "collidable_list.h"
#include "texture.h"
#include "camera.h"
#include "sphere.h"
#include "quad.h"

void checkered_spheres() {
    collidable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(vec3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera_config config = {
        400,            //  int image_width;
        225,            //  int image_height;
        20,             //  double vfov;
        vec3(13, 2, 3), //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        100,            //  int samples_per_pixel;
        50,             //  int max_depth;
        0.6,            //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);
    

    cam.render(world, "checkered_sphere.ppm", std::thread::hardware_concurrency()-1);
}

void perlin_spheres() {
    collidable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(vec3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera_config config = {
        400,            //  int image_width;
        225,            //  int image_height;
        20,             //  double vfov;
        vec3(13, 2, 3), //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        100,            //  int samples_per_pixel;
        50,             //  int max_depth;
        0.6,            //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "perlin_sphere.ppm", std::thread::hardware_concurrency()-1);
}

void quads() {
    collidable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(vec3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(vec3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(vec3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(vec3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));


    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(0, 0, 9),  //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        100,            //  int samples_per_pixel;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "quads.ppm", std::thread::hardware_concurrency()-1);
}

void simple_light() {
    collidable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(vec3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(vec3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));
    world.add(make_shared<sphere>(vec3(0,7,0), 2, difflight));

     camera_config config = {
        400,            //  int image_width;
        225,            //  int image_height;
        20,             //  double vfov;
        vec3(26, 3, 6), //  vec3 lookfrom;
        vec3(0, 2, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        100,            //  int samples_per_pixel;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2,              //  double gamma;
        color(0,0,0)    //  color background;
    };

    camera cam(config);

    cam.render(world, "simple_light.ppm", std::thread::hardware_concurrency());
}


void box() {
    collidable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));
    auto light = make_shared<diffuse_light>(color(15,15,15));

    double box_width = 500;   // x
    double box_height = 500;  // y
    double box_depth = 500;   // z
    double light_width = box_width/5;
    double light_depth = box_depth/5;

    vec3 bottomleft_front_corner = vec3(box_width/2, -box_height/2, -box_depth/2);
    vec3 topright_back_corner = -bottomleft_front_corner;
    vec3 light_origin = topright_back_corner + vec3((box_width-light_width)/2, 0, -(box_depth-light_depth)/2);

    auto floor = make_shared<quad>(bottomleft_front_corner, vec3(0, 0, box_depth), vec3(-box_width, 0, 0), lower_teal);
    auto ceiling = make_shared<quad>(topright_back_corner, vec3(0, 0, -box_depth), vec3(box_width, 0, 0), upper_orange);
    auto ceiling_light = make_shared<quad>(light_origin, vec3(0, 0, -light_depth), vec3(light_width, 0, 0), light);

    auto back_wall = make_shared<quad>(topright_back_corner, vec3(0, -box_height, 0), vec3(box_width, 0, 0), back_green);
    
    auto left_wall = make_shared<quad>(bottomleft_front_corner, vec3(0, box_height, 0), vec3(0, 0, box_depth), left_red);
    auto right_wall = make_shared<quad>(topright_back_corner, vec3(0, -box_height, 0), vec3(0, 0, -box_depth), right_blue);
    

    world.add(floor);
    world.add(ceiling);
    world.add(ceiling_light);
    world.add(back_wall);
    world.add(left_wall);
    world.add(right_wall);

    // shared_ptr<sphere> centerpiece = make_shared<sphere>(vec3(0, 0, 4), 1, left_red);
    // world.add(centerpiece);

    // Quads
    // world.add(make_shared<quad>(vec3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    // world.add(make_shared<quad>(vec3( 3,-2, 2), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    // world.add(make_shared<quad>(vec3(-2, 3, 2), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    // world.add(make_shared<quad>(vec3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));


    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(0, 0, -box_depth),  //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        200,            //  int samples_per_pixel;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "box.ppm", std::thread::hardware_concurrency());
}

int main() {
    switch (7) {
        // case 1: bouncing_spheres();   break;
        case 2: checkered_spheres();  break;
        // case 3: earth();              break;
        case 4: perlin_spheres();   break;
        case 5: quads();            break;
        case 6: simple_light();     break;
        case 7: box();              break;
    }
}