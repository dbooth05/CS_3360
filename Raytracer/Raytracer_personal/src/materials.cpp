#include "materials.hpp"

bool lamber::scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const {
    auto scatter_dir = rec.norm + random_unit_vector();

    if (scatter_dir.near_zero()) {
        scatter_dir = rec.norm;
    }

    scattered = ray(rec.p, scatter_dir, r.time());
    atten = tex->value(rec.u, rec.v, rec.p);
    return true;
}

bool metal::scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const {
    vec3 reflected = unit_vector(reflect(r.direction(), rec.norm)) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected, r.time());
    atten = albedo;
    return dot(scattered.direction(), rec.norm) > 0;
}

bool dialectric::scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const {
    atten = color(1.0, 1.0, 1.0);
    double ri = rec.facing ? (1.0 / refrac_idx) : refrac_idx;

    vec3 unit_dir = unit_vector(r.direction());

    double cos_theta = std::fmin(dot(-unit_dir, rec.norm), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    vec3 dir;
    if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) {
        dir = reflect(unit_dir, rec.norm);
    } else {
        dir = refract(unit_dir, rec.norm, ri);
    }

    scattered = ray(rec.p, dir, r.time());

    return true;
}

double dialectric::reflectance(double cos, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cos), 5);
}

bool isotropic::scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const {
    scattered = ray(rec.p, random_unit_vector(), r.time());
    atten = tex->value(rec.u, rec.v, rec.p);
    return true;
}