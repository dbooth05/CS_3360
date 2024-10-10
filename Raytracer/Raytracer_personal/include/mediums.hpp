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

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

        axis_bound_box bounding_box() const override;

    private:
        shared_ptr<hittable> boundary;
        double neg_inv_dens;
        shared_ptr<material> phase;
};

#endif