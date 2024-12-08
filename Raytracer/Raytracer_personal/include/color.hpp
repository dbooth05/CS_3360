#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include "interval.hpp"
#include "perlin.hpp"
#include "rtw_stb_image.hpp"

#include <iostream>
#include <memory>
#include <algorithm>

using std::shared_ptr;
using std::make_shared;

using color = vec3;

double pi_col = 3.14159265;

inline double lin_to_gamma(double lin_comp) {
    if (lin_comp > 0) {
        return std::sqrt(lin_comp);
    }

    return 0;
}

std::string write_color(color &col, int aa, bool is_hdr=false, double gamma=1.0) {

    if (is_hdr) {
        return std::to_string(col.x()) + ' ' + std::to_string(col.y()) + ' ' + std::to_string(col.z()) + '\n';
    }

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

class image_hdr_tex : public texture {
    public:
        const static int bytes_per_pixel = 3;

        image_hdr_tex(const char* filename) {
            auto comp_per_pix = 3;
            img_data = stbi_loadf(filename, &width, &height, &comp_per_pix, comp_per_pix);

            if (img_data) {
                std::clog << "\nLoading hdr image: " << filename << std::endl;
            } else {
                std::cerr << "\nNot an hdr image: " << filename << std::flush;
                exit(0);
            }

            bytes_per_line = bytes_per_pixel * width;
        }

        ~image_hdr_tex() {
            if (img_data) {
                stbi_image_free(img_data);
            }
        }        

        color value(double u, double v, const vec3 &p) const override {
            if (!img_data || height <= 0) return color(0, 1, 1);

           u = std::clamp(u, 0.0, 1.0);
           v = 1 - std::clamp(v, 0.0, 1.0);

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            if (i >= width) i = width - 1;
            if (j >= height) j = height - 1;


            float* pix = img_data + j * bytes_per_line + i * bytes_per_pixel;

            return color(pix[0], pix[1], pix[2]);            
        }

    private:
        int width;
        int height;
        int bytes_per_line;
        float *img_data = nullptr;
};

static void get_spherical_uv(const vec3 &p, double& u, double&v){
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi_col;

    u = phi / (2*pi_col);
    v = theta / pi_col;
}

#endif