#ifndef PDF_HPP
#define PDF_HPP

#include "vec3.hpp"
#include "onb.hpp"

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

#endif