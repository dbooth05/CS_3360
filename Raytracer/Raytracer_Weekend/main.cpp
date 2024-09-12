#include <stdio.h>
#include <stdint.h>

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#define X 768
#define Y 768

int main(int argc, char *argv[]) {

    // World
    hittable_list world;

    auto mat_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto mat_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto mat_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto mat_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, mat_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, mat_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, mat_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, mat_right));

    camera cam;

    cam.aspect_ratio        = 4.0 / 3.0;
    cam.image_width         = X;
    cam.samples_per_pixel   = 100;
    cam.max_depth           = 50;

    cam.render(world);

    return 0;
}