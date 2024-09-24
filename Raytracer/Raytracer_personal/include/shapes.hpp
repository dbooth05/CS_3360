#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "constants.hpp"

class sphere : public hittable {
    public:
        // static sphere, not moving
        sphere(const vec3 &center, double rad, shared_ptr<material> mat) : center(center, vec3(0, 0, 0)), rad(std::fmax(0, rad)), mat(mat) {
            auto rvec = vec3(rad, rad, rad);
            bound_box = axis_bound_box(center - rvec, center + rvec);
        }

        // moving sphere object
        sphere(const vec3 &c1, const vec3 &c2, double rad, shared_ptr<material> mat) : center(c1, c2 - c1), rad(std::fmax(0, rad)), mat(mat) {
            auto rvec = vec3(rad, rad, rad);
            axis_bound_box b1(center.at(0) - rvec, center.at(0) + rvec);
            axis_bound_box b2(center.at(1) - rvec, center.at(1) + rvec);
            bound_box = axis_bound_box(b1, b2);
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        ray center;
        double rad;
        shared_ptr<material> mat;
        axis_bound_box bound_box;
};

#endif