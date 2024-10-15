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
    auto denominator = dot(norm, r.direction());

    if (std::fabs(denominator) < 1e-8) {
        return false;
    }

    auto t = (D - dot(norm, r.origin())) / denominator;
    if (!inter.contains(t)) {
        return false;
    }

    auto intersection = r.at(t);

    vec3 planar_hit_vec = intersection - Q;
    auto alpha = dot(w, cross(planar_hit_vec, v));
    auto beta = dot(w, cross(u, planar_hit_vec));

    if (!is_interior(alpha, beta, rec)) {
        return false;
    }

    rec.t = t;
    rec.p = intersection;
    rec.mat = mat;
    rec.set_facing(r, norm);

    return true;
}

bool quad::is_interior(double a, double b, hit_record &rec) const {
    interval unit_interval = interval(0, 1);

    if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}

shared_ptr<hittable_list> box(const vec3 &a, const vec3 &b, shared_ptr<material> mat) {

    auto sides = make_shared<hittable_list>();

    auto min = vec3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = vec3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(vec3(min.x(), min.y(), max.z()), dx, dy, mat));    // front
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -dz, dy, mat));   // right
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -dx, dy, mat));   // back
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dz, dy, mat));    // left
    sides->add(make_shared<quad>(vec3(min.x(), max.y(), max.z()), dx, -dz, mat));   // top
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dx, dz, mat));    // bottom

    return sides;
}


// void triangle::set_bounding_box() {

// }