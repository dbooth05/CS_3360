#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;

class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_norm) {
            // sets the hit record normal vector
            // NOTE: param 'outward_norm' assumed unit length

            front_face = dot(r.direction(), outward_norm) < 0;
            normal = front_face ? outward_norm : -outward_norm;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif