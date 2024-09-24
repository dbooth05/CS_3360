#ifndef AXIS_BOUNDING_BOX_HPP
#define AXIS_BOUNDING_BOX_HPP

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class axis_bound_box {
    public:
        interval x, y, z;

        axis_bound_box() {}

        axis_bound_box(const interval &x, const interval &y, const interval &z) : x(x), y(y), z(z) {}

        axis_bound_box(const vec3 &u, const vec3 &v);

        axis_bound_box(const axis_bound_box &b0, const axis_bound_box &b1);

        const interval &axis_interval(int n) const;

        bool hit(const ray &r, interval i) const;

        int longest_axis() const;

        static const axis_bound_box empty, universe;

    private:

};

#endif