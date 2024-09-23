#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "hittable.hpp"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const ray &r_in, const hit_record& rec, color &attenuation, ray &scattered
        ) const {
            return false;
        }
};

class lamber : public material {
    public:
        lamber(const color &albedo) : albedo(albedo) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override {
            auto scatter_dir = rec.norm + random_unit_vector();

            if (scatter_dir.near_zero()) {
                scatter_dir = rec.norm;
            }

            scattered = ray(rec.p, scatter_dir);
            atten = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override {
            vec3 reflected = unit_vector(reflect(r.direction(), rec.norm)) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            atten = albedo;
            return dot(scattered.direction(), rec.norm) > 0;
        }

    private:
        color albedo;
        double fuzz;
};

#endif