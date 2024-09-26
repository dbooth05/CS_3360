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
        lamber(const color &albedo) : tex(make_shared<solid_color>(albedo)) {}
        lamber(const shared_ptr<texture> tex) : tex(tex) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override;

    private:
        shared_ptr<texture> tex;
};

class metal : public material {
    public:
        metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override;

    private:
        color albedo;
        double fuzz;
};

class dialectric : public material {
    public:
        dialectric(double refrac_idx) : refrac_idx(refrac_idx) {};

        bool scatter(const ray&r, const hit_record &rec, color &atten, ray &scattered) const override;

    private:
        double refrac_idx;

        static double reflectance(double cos, double ref_idx);
};

#endif