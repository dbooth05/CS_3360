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

        virtual double pdf_value(const vec3 &orig, const vec3 &dir) const {
            return 0.0;
        }

        virtual vec3 random(const vec3 &orig) const {
            return vec3(1, 0, 0);
        }
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
        rotate_y(shared_ptr<hittable> obj, double ang) : obj(obj) {
            auto rad = degrees_to_rad(ang);
            s_th = std::sin(rad);
            c_th = std::cos(rad);

            bound_box = obj->bounding_box();

            vec3 min(h_inf, h_inf, h_inf);
            vec3 max(-h_inf, -h_inf, -h_inf);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i * bound_box.x.max + (1 - i) * bound_box.x.min;
                        auto y = j * bound_box.y.max + (1 - j) * bound_box.y.min;
                        auto z = k * bound_box.z.max + (1 - k) * bound_box.z.min;

                        auto new_x = c_th * x + s_th * z;
                        auto new_z = -s_th * x + c_th * z;

                        vec3 test(new_x, y, new_z);
                        for (int c = 0; c < 3; c++) {
                            min[c] = std::fmin(min[c], test[c]);
                            max[c] = std::fmax(max[c], test[c]);
                        }
                    }
                }
            }
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            auto orig = vec3(
                (c_th * r.origin().x()) - (s_th * r.origin().z()),
                r.origin().y(),
                (s_th * r.origin().x()) + (c_th * r.origin().z())
            );

            auto dir = vec3(
                (c_th * r.direction().x()) - (s_th * r.direction().z()),
                r.direction().y(),
                (s_th * r.direction().x()) + (c_th * r.direction().z())
            );

            ray rotated(orig, dir, r.time());

            if (!obj->hit(rotated, inter, rec)) {
                return false;
            }

            rec.p = vec3(
                (c_th * rec.p.x()) + (s_th * rec.p.z()),
                rec.p.y(),
                (-s_th * rec.p.x()) + (c_th * rec.p.z())
            );

            rec.norm = vec3(
                (c_th * rec.norm.x()) + (s_th * rec.norm.z()),
                rec.norm.y(),
                (-s_th * rec.norm.x()) + (c_th * rec.norm.z())
            );

            return true;
        }

        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        shared_ptr<hittable> obj;
        double s_th, c_th;
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