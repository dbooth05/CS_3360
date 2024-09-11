#include <stdio.h>
#include <stdint.h>

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#define X 1024
#define Y 768

int main(int argc, char *argv[]) {

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 4.0 / 3.0;
    cam.image_width = X;
    cam.samples_per_pixel = 100;

    cam.render(world);

    return 0;
}