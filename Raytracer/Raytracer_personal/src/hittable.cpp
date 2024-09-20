#include "hittable.hpp"

void hit_record::set_facing(const ray &r, const vec3 &out) {
    facing = dot(r.direction(), out) < 0;
    norm = facing ? out : -out;
}

hittable_list::hittable_list(vector<shared_ptr<hittable>> n_objs) {
    for (shared_ptr<hittable> obj : n_objs) {
        hittable_list::add(obj);
    }
}

bool hittable_list::hit(const ray &r, double ray_min, double ray_max, hit_record &rec) const {
    hit_record temp;
    bool hits = false;
    auto closest_hit = ray_max;

    for (const auto &obj : objs) {
        if (obj->hit(r, ray_min, closest_hit, temp)) {
            hits = true;
            closest_hit = temp.t;
            rec = temp;
        }
    }

    return hits;
}