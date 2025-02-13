#pragma once

#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>
#include <cmath>
#include <cstdlib>

const double pi_vec3 = 3.14159265;

inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

class vec3 {
    public:
        float e[3];

        // constructors
        vec3() : e{0, 0, 0} {}
        vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        // position getter functions
        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }

        // utility functions
        double len() const { return std::sqrt(len_sqrd()); }
        double len_sqrd() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
        bool near_zero() const {
            auto s = 1e-8;
            return (std::fabs(e[0] < s)) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        // operator overload functions
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }
};

// non-class utility functions
std::ostream& operator<<(std::ostream& out, const vec3& v);
vec3 operator+(const vec3& u, const vec3& v);
vec3 operator+(const vec3& u, double t);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator*(double t, const vec3& v);
vec3 operator*(const vec3& v, double t);
vec3 operator/(const vec3& v, double t);
vec3 operator/(const vec3& u, const vec3& v);
double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(const vec3& v);
vec3 random_in_unit_disk();
vec3 random_unit_vector();
vec3 random_on_hemisphere(const vec3& normal);
vec3 reflect(const vec3& v, const vec3& n);
vec3 refract(const vec3& uv, const vec3& n, double t);
vec3 random_cos_dir();
vec3 normalize(const vec3 &v);


#endif