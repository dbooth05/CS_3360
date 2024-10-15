#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "constants.hpp"

class sphere : public hittable {
    public:
        // static sphere, not moving
        sphere(const vec3 &center, double rad, shared_ptr<material> mat) : center(center, vec3(0, 0, 0)), rad(std::fmax(0, rad)), mat(mat) {
            auto rvec = vec3(rad, rad, rad);
            bound_box = axis_bound_box(center - rvec, center + rvec);
        }

        // moving sphere object
        sphere(const vec3 &c1, const vec3 &c2, double rad, shared_ptr<material> mat) : center(c1, c2 - c1), rad(std::fmax(0, rad)), mat(mat) {
            auto rvec = vec3(rad, rad, rad);
            axis_bound_box b1(center.at(0) - rvec, center.at(0) + rvec);
            axis_bound_box b2(center.at(1) - rvec, center.at(1) + rvec);
            bound_box = axis_bound_box(b1, b2);
        }

        static void get_sphere_uv(const vec3 &p, double &u, double &v);

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

        axis_bound_box bounding_box() const override { return bound_box; }

    private:
        ray center;
        double rad;
        shared_ptr<material> mat;
        axis_bound_box bound_box;
};

class quad : public hittable {
    public:
        quad(const vec3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat) { 
            auto n = cross(u, v);
            norm = unit_vector(n);
            D = dot(norm, Q);
            w = n / dot(n, n); 

            set_bounding_box(); 
        }

        virtual void set_bounding_box();

        axis_bound_box bounding_box() const override { return bound_box; }

        bool hit(const ray &r, interval inter, hit_record &rec) const override;

        virtual bool is_interior(double a, double b, hit_record &rec) const;

    private:
        vec3 Q, u, v, w;
        shared_ptr<material> mat;
        axis_bound_box bound_box;
        vec3 norm;
        double D;
};

shared_ptr<hittable_list> box(const vec3 &a, const vec3 &b, shared_ptr<material> mat);

// class triangle : public hittable {
//     public:
//         triangle(const vec3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat) {
//             auto n = cross(u, v);
//             norm = unit_vector(n);
//             D = dot(norm, Q);
//             w = n / dot(n, n);

//             set_bounding_box();
//         }

//         virtual void set_bounding_box();

//         axis_bound_box bounding_box() const override { return bound_box; }

//         bool hit(const ray &r, interval inter, hit_record &rec) const override;

//         virtual bool is_interior(double a, double b, hit_record &rec) const;

//     private:
//         vec3 Q, u, v, w;
//         shared_ptr<material> mat;
//         axis_bound_box bound_box;
//         vec3 norm;
//         double D;
// };

#endif