#ifndef PDF_HPP
#define PDF_HPP

#include "vec3.hpp"
#include "onb.hpp"
#include "hittable.hpp"

class pdf {
    public:
        virtual ~pdf() {}

        virtual double value(const vec3 &dir) const = 0;
        virtual vec3 generate() const = 0;
};

class sphere_pdf : public pdf {
    public:
        sphere_pdf() {}

        double value(const vec3 &dir) const override {
            return 1 / (4 * pi_vec3);
        }

        vec3 generate() const override {
            return random_unit_vector();
        }
};

class cos_pdf : public pdf {
    public:
        cos_pdf(const vec3 &w) : uvw(w) {}

        double value(const vec3 &dir) const override {
            auto cos_theta = dot(unit_vector(dir), uvw.w());
            return std::fmax(0, cos_theta / pi_vec3);
        }

        vec3 generate() const override {
            return uvw.transform(random_cos_dir());
        }

    private:
        onb uvw;
};

class hittable_pdf : public pdf {
    public:
        hittable_pdf(const hittable &objs, const vec3 &orig) : objs(objs), orig(orig) {}

        double value(const vec3 &dir) const override {
            return objs.pdf_value(orig, dir);
        }

        vec3 generate() const override {
            return objs.random(orig);
        }

    private:
        const hittable &objs;
        vec3 orig;
};

class mix_pdf : public pdf {
    public:
        mix_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
            p[0] = p0;
            p[1] = p1;
        }

        double value(const vec3& dir) const override {
            return 0.5 * p[0]->value(dir) + 0.5 * p[1]->value(dir);
        }

        vec3 generate() const override {
            if (random_double() < 0.5) return p[0]->generate();
            else return p[1]->generate();
        }

    private:
        shared_ptr<pdf> p[2];
};

#endif