#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>

#include "axis-bounding-box.hpp"
#include "hittable.hpp"
#include "constants.hpp"

class bvh_node : public hittable {
    public:
        interval x, y, z;

        bvh_node(hittable_list list) : bvh_node(list.objs, 0, list.objs.size()) {}

        bvh_node(std::vector<shared_ptr<hittable>> &objs, size_t start, size_t end);

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

        axis_bound_box bounding_box() const override { return bound_box; } 

        static const axis_bound_box empty, universe;

    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        axis_bound_box bound_box;

        static bool box_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_idx);
        static bool box_x_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
        static bool box_y_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
        static bool box_z_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
};

#endif