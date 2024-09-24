#include "axis-bounding-box.hpp"

axis_bound_box::axis_bound_box(const vec3 &u, const vec3 &v) {
    x = (u[0] <= v[0]) ? interval(u[0], v[0]) : interval(v[0], u[0]);
    y = (u[1] <= v[1]) ? interval(u[1], v[1]) : interval(v[1], u[1]);
    z = (u[2] <= v[2]) ? interval(u[2], v[2]) : interval(v[2], u[2]);
}

axis_bound_box::axis_bound_box(const axis_bound_box &b0, const axis_bound_box &b1) {
    x = interval(b0.x, b1.x);
    y = interval(b0.y, b1.y);
    z = interval(b0.z, b1.z);
}

const interval &axis_bound_box::axis_interval(int n) const {
    if (n == 0) return x;
    if (n == 1) return y;
    return z;
}

bool axis_bound_box::hit(const ray &r, interval i) const {
    const vec3 &orig = r.origin();
    const vec3 &dir = r.direction();

    for (int a = 0; a < 3; a++) {
        const interval &ax = axis_interval(a);
        const double ad = 1.0 / dir[a];

        auto t0 = (ax.min - orig[a]) * ad;
        auto t1 = (ax.max - orig[a]) * ad;

        if (t0 < t1) {
            if (t0 > i.min) i.min = t0;
            if (t1 < i.max) i.max = t1;
        } else {
            if (t1 > i.min) i.min = t1;
            if (t0 < i.max) i.max = t0;
        }

        if (i.max <= i.min) {
            return false;
        }
    }

    return true;
}

int axis_bound_box::longest_axis() const  {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    } else {
        return y.size() > z.size() ? 1 : 2;
    }
}

const axis_bound_box axis_bound_box::empty = axis_bound_box(interval::empty, interval::empty, interval::empty);
const axis_bound_box axis_bound_box::universe = axis_bound_box(interval::universe, interval::universe, interval::universe);