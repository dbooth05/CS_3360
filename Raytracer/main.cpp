#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include "color.h"
#include "vec3.h"

#define X 256
#define Y 256

#define R 0
#define G 1
#define B 2

int main(int argc, char *argv[]) {

    // Render
    std::cout << "P3\n" << X << ' ' << Y << "\n255\n";

    for (int j = 0; j < Y; j++) {
        std::clog << "\rScanlines remaining: " << (Y - j) << ' ' << std::flush;
        for (int i = 0; i < X; i++) {
            auto pixel_color = color(double(i)/(X-1), double(j)/(Y-1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.\n";

    return 0;
}