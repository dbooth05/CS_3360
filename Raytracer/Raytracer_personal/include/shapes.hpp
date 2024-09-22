#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "constants.hpp"

class shapes : public hittable {
    public:
        // default constructor: should do nothing
        shapes() {}
    private:
        vec3 center_pos;
};

class sphere : public shapes {
    public:
        sphere(const vec3 &center, double rad) : center(center), rad(std::fmax(0, rad)) {}

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

    private:
        vec3 center;
        double rad;
};

#endif