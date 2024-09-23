#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "constants.hpp"

class sphere : public hittable {
    public:
        sphere(const vec3 &center, double rad, shared_ptr<material> mat) : center(center), rad(std::fmax(0, rad)), mat(mat) {}

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

    private:
        vec3 center;
        double rad;
        shared_ptr<material> mat;
};

#endif