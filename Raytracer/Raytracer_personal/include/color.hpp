#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"
#include "perlin.hpp"

#include <iostream>
#include <memory>

using std::shared_ptr;
using std::make_shared;

#define R 0
#define G 1
#define B 2

using color = vec3;

inline double lin_to_gamma(double lin_comp);

std::string write_color(color &col);

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const vec3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const color& albedo) : albedo(albedo) {}

        solid_color(double r, double g, double b) : solid_color(color(r,g,b)) {}

        color value(double u, double v, const vec3 &p) const override { return albedo; }

    private:
        color albedo;
};

class checkers : public texture {
    public:
        checkers(double scale, shared_ptr<texture> even, shared_ptr<texture> odd) 
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

        checkers(double scale, const color &col1, const color &col2)
            : checkers(scale, make_shared<solid_color>(col1), make_shared<solid_color>(col2)) {}

        color value(double u, double v, const vec3 &p) const override;

    private:
        double inv_scale;
        shared_ptr<texture> even, odd;
};

class noise_tex : public texture {
    public:
        noise_tex(double scale) : scale(scale) {}

        color value(double u, double v, const vec3 &p) const override;

    private:
        perlin noise;
        double scale;
};

#endif