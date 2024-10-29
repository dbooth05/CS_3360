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
const int default_width = 1024;
const double default_aspect = 16.0 / 9.0;
const int default_max_depth = 15;
const int default_anti_alias = 15;
const int default_fov = 90;

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

inline int rand_int(int min, int max) {
    return int(rand_double(min, max+1));
}

// common use src/header files
#include "color.hpp"
// #include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "shapes.hpp"

#endif