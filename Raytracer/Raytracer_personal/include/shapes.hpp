#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <optional>

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

        static void get_sphere_uv(const vec3 &p, double &u, double &v) {
            auto theta = std::acos(-p.y());
            auto phi = std::atan2(-p.z(), p.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {

            vec3 cur_center = center.at(r.time());
            vec3 oc = cur_center - r.origin();

            auto a = r.direction().len_sqrd();
            auto h = dot(r.direction(), oc);
            auto c = oc.len_sqrd() - rad * rad;
            auto disc = h * h - a * c;

            if (disc < 0) {
                return false;
            }

            auto rt = (h - std::sqrt(disc)) / a;

            if (!inter.surrounds(rt)) {
                rt = (h + std::sqrt(disc)) / a;
                if (!inter.surrounds(rt)) {
                    return false;
                }
            }

            rec.t = rt;
            rec.p = r.at(rec.t);
            vec3 out = (rec.p - cur_center) / rad;
            rec.set_facing(r, out);
            get_sphere_uv(out, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

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

        virtual void set_bounding_box() {
            auto bound_box_diag1 = axis_bound_box(Q, Q + u + v);
            auto bound_box_diag2 = axis_bound_box(Q + u, Q + v);
            bound_box = axis_bound_box(bound_box_diag1, bound_box_diag2);
        }

        axis_bound_box bounding_box() const override { return bound_box; }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            auto denominator = dot(norm, r.direction());

            if (std::fabs(denominator) < 1e-8) {
                return false;
            }

            auto t = (D - dot(norm, r.origin())) / denominator;
            if (!inter.contains(t)) {
                return false;
            }

            auto intersection = r.at(t);

            vec3 planar_hit_vec = intersection - Q;
            auto alpha = dot(w, cross(planar_hit_vec, v));
            auto beta = dot(w, cross(u, planar_hit_vec));

            if (!is_interior(alpha, beta, rec)) {
                return false;
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_facing(r, norm);

            return true;
        }

        virtual bool is_interior(double a, double b, hit_record &rec) const {
            interval unit_interval = interval(0, 1);

            if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        vec3 Q, u, v, w;
        shared_ptr<material> mat;
        axis_bound_box bound_box;
        vec3 norm;
        double D;
};

shared_ptr<hittable_list> box(const vec3 &a, const vec3 &b, shared_ptr<material> mat) {

    auto sides = make_shared<hittable_list>();

    auto min = vec3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = vec3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(vec3(min.x(), min.y(), max.z()), dx, dy, mat));    // front
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), max.z()), -dz, dy, mat));   // right
    sides->add(make_shared<quad>(vec3(max.x(), min.y(), min.z()), -dx, dy, mat));   // back
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dz, dy, mat));    // left
    sides->add(make_shared<quad>(vec3(min.x(), max.y(), max.z()), dx, -dz, mat));   // top
    sides->add(make_shared<quad>(vec3(min.x(), min.y(), min.z()), dx, dz, mat));    // bottom

    return sides;  
}

class triangle : public hittable {
    public:
        triangle(const vec3 &a, const vec3 &b, const vec3 &c, shared_ptr<material> mat) 
            : v0(a), v1(b), v2(c), mat(mat) {
                set_bounding_box();
        }

        bool hit(const ray &r, interval inter, hit_record &rec) const override {
            double epsilon = 1e-8;
            vec3 e1 = v1 - v0;
            vec3 e2 = v2 - v0;

            vec3 h = cross(r.direction(), e2);
            double a = dot(e1, h);

            if (a > -epsilon && a < epsilon) return false;

            double f = 1.0 / a;
            vec3 s = r.origin() - v0;
            double u = f * dot(s, h);
            if (u < 0.0 || u > 1.0) return false;

            vec3 q = cross(s, e1);
            double v = f * dot(r.direction(), q);
            if (v < 0.0 || u + v > 1.0) return false;

            double t = f * dot(e2, q);
            if (t < inter.min || t > inter.max) return false;

            rec.t = t;
            rec.p = r.origin() + r.direction() * t;
            vec3 norm = cross(e1, e2);
            rec.set_facing(r, norm);
            rec.mat = mat;

            return true;          
        }

        virtual void set_bounding_box() {
            auto min = vec3(
                std::min(v0.x(), std::min(v1.x(), v2.x())),
                std::min(v0.y(), std::min(v1.y(), v2.y())),
                std::min(v0.z(), std::min(v1.z(), v2.z()))
            );

            auto max = vec3(
                std::max(v0.x(), std::max(v1.x(), v2.x())),
                std::max(v0.y(), std::max(v1.y(), v2.y())),
                std::max(v0.z(), std::max(v1.z(), v2.z()))
            );

            bound_box = axis_bound_box(min, max);
        }

        axis_bound_box bounding_box() const override {
            return bound_box;
        }

        vec3 v0, v1, v2;

    private:
        shared_ptr<material> mat;
        axis_bound_box bound_box;
};

class triangle_mesh : public hittable {
public:
    triangle_mesh(const std::vector<triangle>& triangles, std::shared_ptr<material> mat) 
        : triangles(triangles), mat(mat) {
        set_bounding_box();
    }

    bool hit(const ray &r, interval inter, hit_record &rec) const override {
        bool hit_anything = false;
        hit_record temp_rec;
        double closest_so_far = inter.max;

        for (const auto& triangle : triangles) {
            if (triangle.hit(r, interval(inter.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec; // Store the closest hit record
            }
        }
        return hit_anything;
    }

    axis_bound_box bounding_box() const override {
        return bound_box;
    }

    private:
        std::vector<triangle> triangles;  // Store triangles
        axis_bound_box bound_box;          // Bounding box for the entire mesh
        std::shared_ptr<material> mat;     // Material for the mesh

        void set_bounding_box() {
            // Initialize min and max to extreme values
            vec3 min(std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::max());

            vec3 max(std::numeric_limits<double>::lowest(),
                    std::numeric_limits<double>::lowest(),
                    std::numeric_limits<double>::lowest());

            for (const auto& triangle : triangles) {
                auto tri_bbox = triangle.bounding_box();
                min = vec3(
                    std::min(min.x(), tri_bbox.min.x()),
                    std::min(min.y(), tri_bbox.min.y()),
                    std::min(min.z(), tri_bbox.min.z())
                );

                max = vec3(
                    std::max(max.x(), tri_bbox.max.x()),
                    std::max(max.y(), tri_bbox.max.y()),
                    std::max(max.z(), tri_bbox.max.z())
                );
            }

            bound_box = axis_bound_box(min, max);
        }
};



#endif