#ifndef MEDIUMS_HPP
#define MEDIUMS_HPP

#include <cmath>

#include "hittable.hpp"
#include "materials.hpp"
#include "constants.hpp" // may have to remove this

class medium : public hittable {
    public:
        medium(shared_ptr<hittable> boundary, double dens, shared_ptr<texture> tex)
            : boundary(boundary), neg_inv_dens(-1 / dens), phase(make_shared<isotropic>(tex)) {}

        medium(shared_ptr<hittable> boundary, double dens, const color &albedo)
            : boundary(boundary), neg_inv_dens(-1 / dens), phase(make_shared<isotropic>(albedo)) {}

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            hit_record r1, r2;

            if (!boundary->hit(r, interval::universe, r1)) {
                return false;
            }

            if (!boundary->hit(r, interval(r1.t + 0.0001, INFINITY), r2)) {
                return false;
            }

            if (r1.t < inter.min) r1.t = inter.min;
            if (r2.t > inter.max) r2.t = inter.max;

            if (r1.t >= r2.t) {
                return false;
            }

            auto r_len = r.direction().len();
            auto dist_in_boundary = (r2.t - r1.t) * r_len;
            auto hit_dist = neg_inv_dens * std::log(random_double());

            if (hit_dist > dist_in_boundary) {
                return false;
            }

            rec.t = r1.t + hit_dist / r_len;
            rec.p = r.at(rec.t);

            rec.norm = vec3(1, 0, 0);
            rec.facing = true;
            rec.mat = phase;

            return true;  
        }

        axis_bound_box bounding_box() const override { return boundary->bounding_box(); }

    private:
        shared_ptr<hittable> boundary;
        double neg_inv_dens;
        shared_ptr<material> phase;
};

#endif