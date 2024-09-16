#include <string>
#include <iostream>

#include "../include/vec3.hpp"
#include "../include/camera.hpp"

#define DEFAULT_X = 768
#define DEFAULT_Y = 432



int main(int argc, char* argv[]) {

    // declare camera
    camera cam;

    for (int i = 1; i < argc; i++) {
        std::cout << "Arg[" << i << "]: " << argv[i] << std::endl;

        std::string arg = argv[i];

        if (arg == "option1") {
            // fill these out for camera options
        }
    }


    return 0;
}