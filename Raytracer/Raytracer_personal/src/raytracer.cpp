#include "camera.hpp"
#include "constants.hpp"
#include "materials.hpp"
#include "bvh.hpp"

int main(int argc, char *argv[]) {

    hittable_list world;

    // auto mat_grnd = make_shared<lamber>(color(0.8, 0.8, 0.0));
    // auto mat_cntr = make_shared<lamber>(color(0.1, 0.2, 0.5));
    // auto mat_left = make_shared<dialectric>(1.50);
    // auto mat_bubl = make_shared<dialectric>(1.00 / 1.50);
    // auto mat_rght = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<sphere>(vec3(0.0, -100.5, -1.0), 100.0, mat_grnd));
    // world.add(make_shared<sphere>(vec3(0.0, 0.0, -1.2), 0.5, mat_cntr));
    // world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.5, mat_left));
    // world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.4, mat_bubl));
    // world.add(make_shared<sphere>(vec3(1.0, 0.0, -1.0), 0.5, mat_rght));

    // auto cos_pi = std::cos(pi/4);

    // auto mat_left = make_shared<lamber>(color(0, 0, 1));
    // auto mat_right = make_shared<lamber>(color(1, 0, 0));

    // world.add(make_shared<sphere>(vec3(-cos_pi, 0, -1), cos_pi, mat_left));
    // world.add(make_shared<sphere>(vec3(cos_pi, 0, -1), cos_pi, mat_right));

    auto mat_grnd = make_shared<lamber>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, mat_grnd));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).len() > 0.9) {
                shared_ptr<material> sphere_mat;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_mat = make_shared<lamber>(albedo);
                    auto c2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, c2, 0.2, sphere_mat));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_mat = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                } else {
                    sphere_mat = make_shared<dialectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                }
            }
        }
    }

    auto mat1 = make_shared<dialectric>(1.5);
    auto mat2 = make_shared<lamber>(color(0.4, 0.2, 0.1));
    auto mat3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, mat1));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, mat2));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, mat3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.img_wd = 400;
    cam.aspect = 16.0 / 9.0;
    cam.anti_alias = 50;
    cam.max_depth = 25;

    cam.fov = 20;
    cam.lk_from = vec3(13, 2, 3);
    cam.lk_at = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);

    return 0;
}