#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "hittable.hpp"

class material {
    public:
        virtual ~material() = default;

        virtual color emitted(double u, double v, const vec3 &p) const {
            return color(0, 0, 0);
        }

        virtual bool scatter(
            const ray &r_in, const hit_record& rec, color &attenuation, ray &scattered
        ) const {
            return false;
        }
};

class lamber : public material {
    public:
        lamber(const color &albedo) : tex(make_shared<solid_color>(albedo)) {}
        lamber(const shared_ptr<texture> tex) : tex(tex) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override {
            auto scatter_dir = rec.norm + random_unit_vector();

            if (scatter_dir.near_zero()) {
                scatter_dir = rec.norm;
            }

            scattered = ray(rec.p, scatter_dir, r.time());
            atten = tex->value(rec.u, rec.v, rec.p);
            return true;
        }

    private:
        shared_ptr<texture> tex;
};

class metal : public material {
    public:
        metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override {
            vec3 reflected = unit_vector(reflect(r.direction(), rec.norm)) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected, r.time());
            atten = albedo;
            return dot(scattered.direction(), rec.norm) > 0;
        }

    private:
        color albedo;
        double fuzz;
};

class dialectric : public material {
    public:
        dialectric(double refrac_idx) : refrac_idx(refrac_idx) {};

        bool scatter(const ray&r, const hit_record &rec, color &atten, ray &scattered) const override {
            atten = color(1.0, 1.0, 1.0);
            double ri = rec.facing ? (1.0 / refrac_idx) : refrac_idx;

            vec3 unit_dir = unit_vector(r.direction());

            double cos_theta = std::fmin(dot(-unit_dir, rec.norm), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            vec3 dir;
            if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) {
                dir = reflect(unit_dir, rec.norm);
            } else {
                dir = refract(unit_dir, rec.norm, ri);
            }

            scattered = ray(rec.p, dir, r.time());

            return true;
        }

    private:
        double refrac_idx;

        static double reflectance(double cos, double ref_idx) {
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cos), 5);
        }
};

class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> tex) : tex(tex) {}

        diffuse_light(const color &emit) : tex(make_shared<solid_color>(emit)) {}

        color emitted(double u, double v, const vec3 &p) const override { return tex->value(u, v, p); }

    private:
        shared_ptr<texture> tex;
};

#endif