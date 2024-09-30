#include "shapes.hpp"

void sphere::get_sphere_uv(const vec3 &p, double &u, double &v) {
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}

bool sphere::hit(const ray &r, interval inter, hit_record &rec) const {

    vec3 cur_center = center.at(r.time());
    vec3 oc = cur_center - r.origin();

    auto a = r.direction().len_sqrd();
    auto h = dot(r.direction(), oc);
    auto c = oc.len_sqrd() - rad * rad;
    auto disc = h * h - a * c;

    if (disc < 0) {
        return false;
    }

    auto rt = (h - std::sqrt(disc)) / a;

    if (!inter.surrounds(rt)) {
        rt = (h + std::sqrt(disc)) / a;
        if (!inter.surrounds(rt)) {
            return false;
        }
    }

    rec.t = rt;
    rec.p = r.at(rec.t);
    vec3 out = (rec.p - cur_center) / rad;
    rec.set_facing(r, out);
    get_sphere_uv(out, rec.u, rec.v);
    rec.mat = mat;

    return true;

}

void quad::set_bounding_box() {
    auto bound_box_diag1 = axis_bound_box(Q, Q + u + v);
    auto bound_box_diag2 = axis_bound_box(Q + u, Q + v);
    bound_box = axis_bound_box(bound_box_diag1, bound_box_diag2);
}

bool quad::hit(const ray &r, interval inter, hit_record &rec) const {
    return false;
}