#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>

#define R 0
#define G 1
#define B 2

using color = vec3;

inline double lin_to_gamma(double lin_comp);

void write_color(std::ostream &out, color &col);

#endif