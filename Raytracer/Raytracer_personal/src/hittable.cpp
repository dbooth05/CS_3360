#include "hittable.hpp"

void hittable_list::add(shared_ptr<hittable> obj) {
    objs.push_back(obj);
    bound_box = axis_bound_box(bound_box, obj->bounding_box());
}

void hit_record::set_facing(const ray &r, const vec3 &out) {
    facing = dot(r.direction(), out) < 0;
    norm = facing ? out : -out;
}

bool translate::hit(const ray &r, interval inter, hit_record &rec) const {
    ray off(r.origin() - offset, r.direction(), r.time());
    if (!obj->hit(off, inter, rec)) {
        return false;
    }

    rec.p += offset;
    return true;
}

rotate_y::rotate_y(shared_ptr<hittable> obj, double angle) : obj(obj) {
    auto rad = degrees_to_rad(angle);
    sin_theta = std::sin(rad);
    cos_theta = std::cos(rad);
    bound_box = obj->bounding_box();

    vec3 min(h_inf, h_inf, h_inf);
    vec3 max(-h_inf, -h_inf, -h_inf);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * bound_box.x.max + (1 - i) * bound_box.x.min;
                auto y = j * bound_box.y.max + (1 - i) * bound_box.y.min;
                auto z = k * bound_box.z.max + (1 - i) * bound_box.z.min;

                auto new_x = cos_theta * x + sin_theta * z;
                auto new_z = -sin_theta * x + cos_theta * z;

                vec3 test(new_x, y, new_z);

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], test[c]);
                    max[c] = std::fmax(max[c], test[c]);
                }
            }
        }
    }

    bound_box = axis_bound_box(min, max);
}

bool rotate_y::hit(const ray &r, interval inter, hit_record &rec) const {
    auto orig = vec3((cos_theta * r.origin().x()) - (sin_theta * r.origin().z()), r.origin().y(),
     (sin_theta * r.origin().x()) + (cos_theta * r.origin().z()));

    auto dir = vec3((cos_theta * r.direction().x()) - (sin_theta * r.direction().z()), r.direction().y(),
     (sin_theta * r.direction().x()) + (cos_theta * r.direction().z()));
    
    ray rotated(orig, dir, r.time());

    if (!obj->hit(rotated, inter, rec)) {
        return false;
    }

    rec.p = vec3((cos_theta * rec.p.x()) + (sin_theta * rec.p.z()), rec.p.y(),
     (-sin_theta * rec.p.x()) + (cos_theta * rec.p.z()));

    rec.norm = vec3((cos_theta * rec.norm.x()) + (sin_theta * rec.norm.z()), rec.norm.y(),
     (-sin_theta * rec.norm.x()) + (cos_theta * rec.norm.z()));

    return true;
}

hittable_list::hittable_list(vector<shared_ptr<hittable>> n_objs) {
    for (shared_ptr<hittable> obj : n_objs) {
        hittable_list::add(obj);
    }
}

bool hittable_list::hit(const ray &r, interval inter, hit_record &rec) const {
    hit_record temp;
    bool hits = false;
    auto closest_hit = inter.max;

    for (const auto &obj : objs) {
        if (obj->hit(r, interval(inter.min, closest_hit), temp)) {
            hits = true;
            closest_hit = temp.t;
            rec = temp;
        }
    }

    return hits;
}