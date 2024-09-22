// #ifndef MATERIALS_HPP
// #define MATERIALS_HPP

// #include "hittable.hpp"

// class material {
//     public:
//         virtual ~material() = default;

//         virtual bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const {
//             return false;
//         }
// };

// class lamber : public material {
//     public:
//         lamber(const color &albedo) : albedo(albedo) {}

//         bool scatter(const ray &r, const hit_record &rec, color &atten, ray &scattered) const override {
//             auto scatter_dir = rec.norm + random_unit_vector();

//             if (scatter_dir.near_zero()) {
//                 scatter_dir = rec.norm;
//             }

//             scattered = ray(rec.p, scatter_dir);
//             atten = albedo / random_double();
//             return true;
//         }

//     private:
//         color albedo;
// };

// class metal : public material {
//     public:
//         metal(const color &albedo) : albedo(albedo) {}

//         bool scatter(const ray &r, const hit_record &rec, color & atten, ray &scattered) const override {
//             vec3 reflected = reflect(r.direction(), rec.norm);
//             scattered = ray(rec.p, reflected);
//             atten = albedo / random_double();
//             return true;
//         }

//     private:
//         color albedo;
// };

// class dialec : public material {

// };

// #endif