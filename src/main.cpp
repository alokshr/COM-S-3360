#include "camera.h"
#include "collidable_list.h"
#include "kd_tree.h"
#include "texture.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "obj_parser.h"

void bouncing_spheres() {
    collidable_list world;

    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).mag() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randvec3() * randvec3();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = randvec3(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    
    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material3));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

    camera_config config = {
        1200,           //  int image_width;
        675,            //  int image_height;
        20,             //  double vfov;
        vec3(13, 2, 3), //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    world = collidable_list(make_shared<kd_tree>(world));

    camera cam(config);
    cam.render(world, "bouncing_spheres.ppm", std::thread::hardware_concurrency());
}

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
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0.6,            //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);
    

    cam.render(world, "checkered_sphere.ppm", std::thread::hardware_concurrency()-1);
}

void earth() {
    collidable_list world;
    auto earth_texture = make_shared<image_texture>("resources/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(vec3(0,0,0), 2, earth_surface);

    world.add(globe);

    camera_config config = {
        400,            //  int image_width;
        225,            //  int image_height;
        20,             //  double vfov;
        vec3(0, 0, 12), //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "earth.ppm", std::thread::hardware_concurrency());
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
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
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
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
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

    auto background_tex = make_shared<solid_color>(color(0,0,0));

    cube_map background = cube_map(background_tex);

    camera_config config = {
        400,            //  int image_width;
        225,            //  int image_height;
        20,             //  double vfov;
        vec3(26, 3, 6), //  vec3 lookfrom;
        vec3(0, 2, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2,              //  double gamma;
        background      // cube_map background;
    };

    camera cam(config);

    cam.render(world, "simple_light.ppm", std::thread::hardware_concurrency());
}

void cornell_box() {
    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    // Walls
    world.add(make_shared<quad>(vec3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    // world.add(make_shared<quad>(vec3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    // world.add(make_shared<quad>(vec3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(vec3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    // Inside boxes
    shared_ptr<collidable> box1 = box(vec3(0,0,0), vec3(165,330,165), white);
    box1 = make_shared<rotate>(box1, vec3(0, 1, 0), 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<collidable> box2 = box(vec3(0,0,0), vec3(165,165,165), white);
    box2 = make_shared<rotate>(box2, vec3(0, 1, 0), -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    camera_config config = {
        600,                    //  int image_width;
        600,                    //  int image_height;
        40,                     //  double vfov;
        vec3(278, 278, -800),   //  vec3 lookfrom;
        vec3(278, 278, 0),      //  vec3 lookat;
        vec3(0, 1, 0),          //  vec3 up;
        4,                      //  int samples_per_batch;
        8,                      //  int batches_per_pixel;
        0.05,                   //  double max_tolerance;
        50,                     //  int max_depth;
        0,                      //  double defocus_angle;
        10,                     //  double defocus_dist;
        2                       //  double gamma;
    };

    camera cam(config);

    cam.render(world, "cornell_box.ppm", std::thread::hardware_concurrency());
}

void triangle_test() {
    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    // auto light = make_shared<diffuse_light>(color(15, 15, 15));

    vec3 a = vec3(-0.5, -0.5,  1.0);
    vec3 b = vec3(-0.25, 0.5,  1.0);
    vec3 c = vec3( 0.0,  0.0,  1.0);

    auto tri = make_shared<triangle>(a, b, c, green);
    
    auto q = make_shared<quad>(
        a + vec3(0, 0, 0.01),
        b-a,
        c-a,
        red
    );

    auto sa = make_shared<sphere>(a, 0.01, red);
    auto sb = make_shared<sphere>(b, 0.01, green);
    auto sc = make_shared<sphere>(c, 0.01, white);

    world.add(tri);
    world.add(q);
    world.add(sa);
    world.add(sb);
    world.add(sc);

    camera_config config = {
        400,            //  int image_width;
        500,            //  int image_height;
        80,             //  double vfov;
        vec3(0, 0, 0),  //  vec3 lookfrom;
        vec3(0, 0.125, 1),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "triangle_test.ppm", std::thread::hardware_concurrency());
}

void obj_file_test() {

    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    // auto white = make_shared<lambertian>(color(.73, .73, .73));
    // auto green = make_shared<lambertian>(color(.12, .45, .15));
    // auto blue   = make_shared<lambertian>(color(.1, .1, .65));
    // auto orange = make_shared<lambertian>(color(.73, .2, .05));
    // auto light = make_shared<diffuse_light>(color(15, 15, 15));
    // auto glass = make_shared<dielectric>(1.6);
    auto shiny = make_shared<metal>(color(.73, .73, .73), 0);

    obj_parser p = obj_parser();
    p.parse_obj_file("resources/diamond.obj");

    auto model = p.generate_triangles(shiny);
    
    auto floor = make_shared<quad>(
        vec3(-5000, -5000, -100),
        vec3(10000, 0, 0),
        vec3(0, 10000, 0),
        red
    );
    
    world.add(model);
    world.add(floor);

    world = collidable_list(make_shared<kd_tree>(world));

    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(100, 80, 20),  //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 0, 1),  //  vec3 up;
        4,              //  int samples_per_batch;
        8,              //  int batches_per_pixel;
        0.05,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "obj_file_test.ppm", std::thread::hardware_concurrency());
}

void obj_file_test_comparision() {

    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    // auto white = make_shared<lambertian>(color(.73, .73, .73));
    // auto green = make_shared<lambertian>(color(.12, .45, .15));
    // auto blue   = make_shared<lambertian>(color(.1, .1, .65));
    // auto orange = make_shared<lambertian>(color(.73, .2, .05));
    // auto light = make_shared<diffuse_light>(color(15, 15, 15));
    // auto glass = make_shared<dielectric>(1.6);
    auto shiny = make_shared<metal>(color(.73, .73, .73), 0);
    auto checker_tex = make_shared<checker_texture>(15, color(.2, .3, .1), color(.9, .9, .9));
    auto checker_mat = make_shared<lambertian>(checker_tex);

    
    vec3 v1 = vec3(0.000000E+00, 0.000000E+00, 78.0000);
    vec3 v2 = vec3(45.0000, 45.0000, 0.000000E+00);
    vec3 v3 = vec3(45.0000, -45.0000, 0.000000E+00);
    vec3 v4 = vec3(-45.0000, -45.0000, 0.000000E+00);
    vec3 v5 = vec3(-45.0000, 45.0000, 0.000000E+00);
    vec3 v6 = vec3(0.000000E+00, 0.000000E+00, -78.0000);

    auto t1 = make_shared<triangle>(v1, v2, v3, shiny);
    auto t2 = make_shared<triangle>(v1, v3, v4, shiny);
    auto t3 = make_shared<triangle>(v1, v4, v5, shiny);
    auto t4 = make_shared<triangle>(v1, v5, v2, shiny);
    auto t5 = make_shared<triangle>(v6, v5, v4, shiny);
    auto t6 = make_shared<triangle>(v6, v4, v3, shiny);
    auto t7 = make_shared<triangle>(v6, v3, v2, shiny);
    auto t8 = make_shared<triangle>(v6, v2, v1, shiny);
    auto t9 = make_shared<triangle>(v6, v1, v5, shiny);
    auto floor = make_shared<quad>(
        vec3(-5000, -5000, -100),
        vec3(10000, 0, 0),
        vec3(0, 10000, 0),
        checker_mat
    );
    
    world.add(t1);
    world.add(t2);
    world.add(t3);
    world.add(t4);
    world.add(t5);
    world.add(t6);
    world.add(t7);
    world.add(t8);
    world.add(t9);
    world.add(floor);

    world = collidable_list(make_shared<kd_tree>(world));

    cube_map background = cube_map(tex_image("resources/Earth_cube_map.png"));

    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(100, 80, 20),  //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 0, 1),  //  vec3 up;
        4,              //  int samples_per_batch;
        32,              //  int batches_per_pixel;
        0.005,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2,               //  double gamma;
        background      //  cube_map background;
    };

    camera cam(config);

    cam.render(world, "obj_file_test_comparision.ppm", std::thread::hardware_concurrency());
}

void cube() {
    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    // auto white = make_shared<lambertian>(color(.73, .73, .73));
    // auto green = make_shared<lambertian>(color(.12, .45, .15));
    // auto blue   = make_shared<lambertian>(color(.1, .1, .65));
    // auto orange = make_shared<lambertian>(color(.73, .2, .05));
    // auto light = make_shared<diffuse_light>(color(15, 15, 15));
    // auto glass = make_shared<dielectric>(1.6);
    auto shiny = make_shared<metal>(color(1, .73, .73), 0);
    auto checker_tex = make_shared<checker_texture>(0.6, color(.2, .3, .1), color(.9, .9, .9));
    auto checker_mat = make_shared<lambertian>(checker_tex);
    auto die_tex = make_shared<image_texture>("resources/die-tex.png");
    auto die_mat = make_shared<lambertian>(die_tex);

    obj_parser p = obj_parser();
    p.parse_obj_file("resources/cube-tex.obj");
    

    auto model = p.generate_triangles(die_mat);
    
    auto floor = make_shared<quad>(
        vec3(-5000, -5000, -1),
        vec3(10000, 0, 0),
        vec3(0, 10000, 0),
        checker_mat
    );
    
    world.add(model);
    world.add(floor);

    world = collidable_list(make_shared<kd_tree>(world));

    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(2, 2, 1.5),//  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 0, 1),  //  vec3 up;
        4,              //  int samples_per_batch;
        32,             //  int batches_per_pixel;
        0.005,          //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "cube.ppm", std::thread::hardware_concurrency());
}

void teapot() {

    collidable_list world;

    // Materials
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto shiny = make_shared<metal>(color(.73, .73, .73), 0);
    auto checker_tex = make_shared<checker_texture>(0.6, color(.2, .3, .1), color(.9, .9, .9));
    auto checker_mat = make_shared<lambertian>(checker_tex);

    obj_parser p = obj_parser();
    p.parse_obj_file("resources/teapot.obj");

    auto model = p.generate_triangles(shiny);
    
    auto floor = make_shared<quad>(
        vec3(-5000, -100, -5000),
        vec3(10000, 0, 0),
        vec3(0, 0, 10000),
        checker_mat
    );
    
    world.add(model);
    world.add(floor);

    world = collidable_list(make_shared<kd_tree>(world));

    camera_config config = {
        400,            //  int image_width;
        400,            //  int image_height;
        80,             //  double vfov;
        vec3(100, 100, -200)*0.11,  //  vec3 lookfrom;
        vec3(0, 0, 0),  //  vec3 lookat;
        vec3(0, 1, 0),  //  vec3 up;
        4,              //  int samples_per_batch;
        32,              //  int batches_per_pixel;
        0.005,           //  double max_tolerance;
        50,             //  int max_depth;
        0,              //  double defocus_angle;
        10,             //  double defocus_dist;
        2               //  double gamma;
    };

    camera cam(config);

    cam.render(world, "teapot.ppm", std::thread::hardware_concurrency());
}

void load_demo(int selection) {
    switch (selection) {
        case 1: bouncing_spheres();             break;
        case 2: checkered_spheres();            break;
        case 3: earth();                        break;
        case 4: perlin_spheres();               break;
        case 5: quads();                        break;
        case 6: simple_light();                 break;
        case 7: cornell_box();                  break;
        case 8: triangle_test();                break;
        case 9: obj_file_test();                break;
        case 10: obj_file_test_comparision();   break;
        case 11: cube();   break;
        case 12: teapot();                      break;
        default:                                break;
    }
}

int main(int argc, const char** argv) {

    if (argc < 2) {
        std::cout <<
            "usage: main.exe [demo number] [other demo numbers...]\n"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            "Renders the given demos\n"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
            "\n"
            "Demos: \n"
            "1: Bouncing Spheres\n"
            "2: Checkered Floor\n"
            "3: Earth\n"
            "4: Perlin Sphere\n"
            "5: Quads\n"
            "6: Simple Light\n"
            "7: Cornell Box\n"
            "8: Triangle Test\n"
            "9: .obj File Test\n"
            "10: .obj File Test Replica\n"
            "11: Cube\n"
            "12: Teapot\n"
        << std::endl;
        return 0;
    }

    std::cout << "Loading " << argc-1 << " demo" << (argc == 2 ? "..." : "s...")<< "\n" << std::endl;

    for (int i = 0; i < argc - 1; i++) {
        std::istringstream ss(argv[i+1]);
        int demo_num;
        ss >> demo_num;
        load_demo(demo_num);
        std::cout << std::endl;
    }
}