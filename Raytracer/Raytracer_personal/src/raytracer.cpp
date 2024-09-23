#include "camera.hpp"
#include "constants.hpp"
#include "materials.hpp"

int main(int argc, char *argv[]) {

    hittable_list world;

    // world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    // world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

    auto material_ground = make_shared<lamber>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lamber>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(vec3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.img_wd = 420;
    // cam.img_wd = 768;
    cam.aspect = 4.0/3.0;
    cam.anti_alias = 50;

    cam.render(world);

    return 0;
}