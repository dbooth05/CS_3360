#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <vector>
#include <memory>

#include "vec3.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "color.hpp"
#include "axis-bounding-box.hpp"

using std::make_shared;
using std::shared_ptr;
using std::vector;

inline double degrees_to_rad(double x) {
    return x * 3.14159265 / 180.0;
};

const double h_inf = std::numeric_limits<double>::infinity();

class material;

class hit_record {
    public:
        vec3 p;
        vec3 norm;
        shared_ptr<material> mat;
        double t, u, v;
        bool facing;

        void set_facing(const ray &r, const vec3 &out) {
            facing = dot(r.direction(), out) < 0;
            norm = facing ? out : -out;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray &r, interval inter, hit_record &rec) const = 0;

        virtual axis_bound_box bounding_box() const = 0;
};

class translate : public hittable {
    public:
        translate(shared_ptr<hittable> obj, const vec3 &offset) : obj(obj), offset(offset) {
            bound_box = obj->bounding_box() + offset;
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            ray off(r.origin() - offset, r.direction(), r.time());
            if (!obj->hit(off, inter, rec)) {
                return false;
            }

            rec.p += offset;
            return true;
        }

        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        shared_ptr<hittable> obj;
        vec3 offset;
        axis_bound_box bound_box;
};

class rotate_y : public hittable {
    public:
        rotate_y(shared_ptr<hittable> obj, double angle) {
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

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
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
    
        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        shared_ptr<hittable> obj;
        double sin_theta;
        double cos_theta;
        axis_bound_box bound_box;
};

class hittable_list : public hittable {
    public:
        vector<shared_ptr<hittable>> objs;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> obj) { add(obj); }
        hittable_list(vector<shared_ptr<hittable>> n_objs) {
            for (shared_ptr<hittable> obj : n_objs) {
                hittable_list::add(obj);
            }
        }

        void remove_objs() { objs.clear(); }

        void add(shared_ptr<hittable> obj) {
            objs.push_back(obj);
            bound_box = axis_bound_box(bound_box, obj->bounding_box());
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
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

        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        axis_bound_box bound_box;
};  

#endif