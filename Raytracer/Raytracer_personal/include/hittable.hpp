#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <vector>
#include <memory>

#include "vec3.hpp"
#include "ray.hpp"
#include "interval.hpp"

using std::make_shared;
using std::shared_ptr;
using std::vector;

class hit_record {
    public:
        vec3 p;
        vec3 norm;
        double t;
        bool facing;

        void set_facing(const ray &r, const vec3 &out);
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray &r, interval inter, hit_record &rec) const = 0;
};

class hittable_list : public hittable {
    public:
        vector<shared_ptr<hittable>> objs;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> obj) { add(obj); }
        hittable_list(vector<shared_ptr<hittable>> n_objs);

        void remove_objs() { objs.clear(); }

        void add(shared_ptr<hittable> obj) { objs.push_back(obj); }

        bool hit(const ray &r, interval inter, hit_record &rec) const override;
};  

#endif