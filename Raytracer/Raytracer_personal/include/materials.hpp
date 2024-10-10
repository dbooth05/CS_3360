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

class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> tex) : tex(tex) {}

        diffuse_light(const color &emit) : tex(make_shared<solid_color>(emit)) {}

        color emitted(double u, double v, const vec3 &p) const override { return tex->value(u, v, p); }

    private:
        shared_ptr<texture> tex;
};

class isotropic : public material {
    public:
        isotropic(const color &albedo) : tex(make_shared<solid_color>(albedo)) {}
        isotropic(shared_ptr<texture> tex) : tex(tex) {}

        bool scatter (const ray &r, const hit_record &rec, color &atten, ray &scattered) const override;

    private:
        shared_ptr<texture> tex;
};

#endif