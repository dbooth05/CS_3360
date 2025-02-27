#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>

#include "axis-bounding-box.hpp"
#include "hittable.hpp"
#include "constants.hpp"
#include "interval.hpp"

class bvh_node : public hittable {
    public:
        interval x, y, z;

        bvh_node(hittable_list list) : bvh_node(list.objs, 0, list.objs.size()) {}

        bvh_node(std::vector<shared_ptr<hittable>> &objs, size_t start, size_t end) {
            
            // bound_box = axis_bound_box::empty;
            for (size_t obj_idx = start; obj_idx < end; obj_idx++) {
                bound_box = axis_bound_box(bound_box, objs[obj_idx]->bounding_box());
            }

            int axis = bound_box.longest_axis();

            auto comparator = (axis == 0) ? box_x_cmp
                            : (axis == 1) ? box_y_cmp
                                        : box_z_cmp;

            size_t obj_span = end - start;

            if (obj_span == 1) {
                left = right = objs[start];
            } else if (obj_span == 2) {
                left = objs[start];
                right = objs[start + 1];
            } else {
                std::sort(std::begin(objs) + start, std::begin(objs) + end, comparator);

                auto middle = start + obj_span / 2;
                left = make_shared<bvh_node>(objs, start, middle);
                right = make_shared<bvh_node>(objs, middle, end);
            }

            bound_box = axis_bound_box(left->bounding_box(), right->bounding_box());
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            if (!bound_box.hit(r, inter)) {
                return false;
            }

            bool hit_l = left->hit(r, inter, rec);
            bool hit_r = right->hit(r, interval(inter.min, hit_l ? rec.t : inter.max), rec);

            return hit_l || hit_r;
        }

        axis_bound_box bounding_box() const override { return bound_box; } 

        static const axis_bound_box empty, universe;

    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        axis_bound_box bound_box;

        static bool box_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_idx) {
            auto a_inter = a->bounding_box().axis_interval(axis_idx);
            auto b_inter = b->bounding_box().axis_interval(axis_idx);
            return a_inter.min < b_inter.min;
        }

        static bool box_x_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_cmp(a, b, 0);
        }

        static bool box_y_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_cmp(a, b, 1);
        }

        static bool box_z_cmp(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
            return box_cmp(a, b, 2);
        }

};

#endif