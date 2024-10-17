#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"
#include "perlin.hpp"
#include "rtw_stb_image.hpp"

#include <iostream>
#include <memory>

using std::shared_ptr;
using std::make_shared;

#define R 0
#define G 1
#define B 2

using color = vec3;

inline double lin_to_gamma(double lin_comp) {
    if (lin_comp > 0) {
        return std::sqrt(lin_comp);
    }

    return 0;
}

std::string write_color(color &col) {

    static const interval intens(0.000, 0.999);

    auto r = lin_to_gamma(col.x());
    auto g = lin_to_gamma(col.y());
    auto b = lin_to_gamma(col.z());

    auto r_bt = int(255.999 * intens.clamp(r));
    auto g_bt = int(255.999 * intens.clamp(g));
    auto b_bt = int(255.999 * intens.clamp(b));

    return std::to_string(r_bt) + ' ' + std::to_string(g_bt) + ' '  + std::to_string(b_bt) + '\n';
}

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

        color value(double u, double v, const vec3 &p) const override {
            auto x = int(std::floor(inv_scale * p.x()));
            auto y = int(std::floor(inv_scale * p.y()));
            auto z = int(std::floor(inv_scale * p.z()));

            bool is_even = (x + y + z) % 2 == 0;

            return is_even ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double inv_scale;
        shared_ptr<texture> even, odd;
};

class noise_tex : public texture {
    public:
        noise_tex(double scale) : scale(scale) {}

        color value(double u, double v, const vec3 &p) const override {
            return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turbulence(p, 7)));
        }

    private:
        perlin noise;
        double scale;
};

class image_tex : public texture {
    public:
        image_tex(const char* filename) : img(filename) {}

        color value(double u, double v, const vec3 &p) const override {
            if (img.height() <= 0) return color(0, 1, 1);

            u = interval(0, 1).clamp(u);
            v = 1.0 - interval(0, 1).clamp(v);

            auto i = int(u * img.width());
            auto j = int(v * img.height());
            auto pix = img.pixel_data(i, j);

            auto color_scale = 1.0 / 255.0;
            return color(color_scale * pix[0], color_scale * pix[1], color_scale * pix[2]);
        }

    private:
        rtw_image img;
};

#endif