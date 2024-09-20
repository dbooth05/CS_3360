#include "camera.hpp"
#include "constants.hpp"

int main(int argc, char *argv[]) {

    camera cam;

    cam.img_wd = 768;
    cam.aspect = 4.0/3.0;

    hittable_list world;

    world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    cam.render(world);

    return 0;
}