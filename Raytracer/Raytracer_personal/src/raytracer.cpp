#include "camera.hpp"
#include "constants.hpp"
#include "materials.hpp"
#include "bvh.hpp"

void my_custom_scene(hittable_list &world, camera &cam) {

    auto mat_grnd = make_shared<lamber>(color(.098, 0.0, 0.2));
    auto mirror = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto glass = make_shared<dialectric>(1.33);

    world.add(make_shared<sphere>(vec3(0, -1001, 0), 1000, mat_grnd));
    world.add(make_shared<sphere>(vec3(-1.25, 0, 0), 1.0, glass));
    world.add(make_shared<sphere>(vec3( 1.25, 0, 0), 1.0, mirror));


    auto bg_mat = make_shared<lamber>(color(1.0, 0.0, 0.0));
    for (int i = 0; i < 4; i++) {
        world.add(make_shared<sphere>(vec3(-3 + (i * 2), 0.0, -6), 1.0, bg_mat));
    }


    cam.anti_alias = 500;
    cam.max_depth = 100;
    cam.lk_from = vec3(0, 0, 10);
}

void bouncing_spheres(hittable_list &world) {

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
}

void checkered_spheres(hittable_list &world) {
    
    auto checker = make_shared<checkers>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lamber>(checker)));
    world.add(make_shared<sphere>(vec3(0,  10, 0), 10, make_shared<lamber>(checker)));

}

void perlin_sphere(hittable_list &world, camera &cam) {
    auto perlin = make_shared<noise_tex>(4);
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lamber>(perlin)));
    world.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lamber>(perlin)));

    cam.defocus_angle = 0;
}

void earth_sphere(hittable_list &world, camera cam) {


    cam.lk_from = vec3(0, 0, 12);
    cam.defocus_angle = 0;
}

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();

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

    camera cam;

    cam.img_wd = 768;
    // cam.img_wd = 400;
    cam.aspect = 16.0 / 9.0;
    cam.anti_alias = 100;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lk_from = vec3(13, 2, 3);
    cam.lk_at = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;


    
    switch (3)
    {
        case 0: my_custom_scene(world, cam); break;
        case 1: bouncing_spheres(world); break;
        case 2: checkered_spheres(world); break;
        case 3: perlin_sphere(world, cam); break;
        default:
            break;
    }

    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world);

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::minutes>(end - start);

    std::clog << "\nTime: " << dur.count() << " minutes\n" << std::flush;

    return 0;
}