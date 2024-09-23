#include "camera.hpp"
#include "constants.hpp"
#include "materials.hpp"

int main(int argc, char *argv[]) {

    hittable_list world;

    auto mat_grnd = make_shared<lamber>(color(0.8, 0.8, 0.0));
    auto mat_cntr = make_shared<lamber>(color(0.1, 0.2, 0.5));
    auto mat_left = make_shared<dialectric>(1.50);
    auto mat_bubl = make_shared<dialectric>(1.00 / 1.50);
    auto mat_rght = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(vec3(0.0, -100.5, -1.0), 100.0, mat_grnd));
    world.add(make_shared<sphere>(vec3(0.0, 0.0, -1.2), 0.5, mat_cntr));
    world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.5, mat_left));
    world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.4, mat_bubl));
    world.add(make_shared<sphere>(vec3(1.0, 0.0, -1.0), 0.5, mat_rght));

    camera cam;

    cam.img_wd = 400;
    cam.aspect = 16.0 / 9.0;
    cam.anti_alias = 100;
    cam.max_depth = 50;
    cam.fov = 90;

    cam.lk_from = vec3(-2, 2, 1);
    cam.lk_at = vec3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.render(world);

    return 0;
}