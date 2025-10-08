#include <time.h>

#include "../renderlib.h"
#include "../camera.h"
#include "../collidable.h"
#include "../collidable_list.h"
#include "../sphere.h"

int main(int argc, char const *argv[])
{
    collidable_list world;

    world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    camera_config config = {
        1080,
        720,
        1,
        10
    };

    camera cam(vec3(), config);

    cam.render(world, "camera_img.ppm");
}
