#include "shapes.hpp"

bool sphere::hit(const ray &r, double ray_min, double ray_max, hit_record &rec) const {

    vec3 oc = center - r.origin();
    auto a = r.direction().len_sqrd();
    auto h = dot(r.direction(), oc);
    auto c = oc.len_sqrd() - rad * rad;
    auto disc = h * h - a * c;

    if (disc < 0) {
        return false;
    }

    auto rt = (h - std::sqrt(disc)) / a;

    if (rt <= ray_min || ray_max <= rt) {
        rt = (h + std::sqrt(disc)) / a;
        if (rt <= ray_min || ray_max <= rt) {
            return false;
        }
    }

    rec.t = rt;
    rec.p = r.at(rec.t);
    vec3 out = (rec.p - center) / rad;
    rec.set_facing(r, out);

    return true;

}