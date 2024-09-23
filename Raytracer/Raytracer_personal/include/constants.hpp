#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <cstdlib>

// c++ std usings
using std::make_shared;
using std::shared_ptr;
using std::vector;

// default vales
#define DEFAULT_WIDTH 1024
#define DEFAULT_ASPECT 16.0/9.0
#define DEFAULT_MAX_DEPTH 15
#define DEFAULT_ANTI_ALIAS 15
#define DEFAULT_FOV 90

const double pi = 3.14159265;
const double inf = std::numeric_limits<double>::infinity();

inline double deg_to_rad(double degrees) {
    return degrees * pi / 180.0;
};

inline double rand_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double rand_double(double min, double max) {
    return min + (max - min) * rand_double();
}

// common use src/header files
#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "shapes.hpp"

#endif