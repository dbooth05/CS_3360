#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "constants.hpp"

class sphere : public hittable {
    public:
        // static sphere, not moving
        sphere(const vec3 &center, double rad, shared_ptr<material> mat) : center(center, vec3(0, 0, 0)), rad(std::fmax(0, rad)), mat(mat) {}

        // moving sphere object
        sphere(const vec3 &c1, const vec3 &c2, double rad, shared_ptr<material> mat) : center(c1, c2 - c1), rad(std::fmax(0, rad)), mat(mat) {}

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

    private:
        ray center;
        double rad;
        shared_ptr<material> mat;
};

#endif