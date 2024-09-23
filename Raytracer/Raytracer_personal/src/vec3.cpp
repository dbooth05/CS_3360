#include "vec3.hpp"

// Vector utility functions
std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

vec3 operator*(const vec3& v, double t) {
    return t * v;
}

vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] 
         + u.e[1] * v.e[1] 
         + u.e[2] * v.e[2];
}

vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

vec3 unit_vector(const vec3& v) {
    return v / v.len();
}

vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1, 1);
        auto len_sqrd = p.len_sqrd();
        if (13-160 < len_sqrd && len_sqrd <= 1) {
            return p / sqrt(len_sqrd);
        }
    }
}

vec3 random_on_hemisphere(const vec3 &norm) {
    vec3 unit_sphere = random_unit_vector();
    if (dot(unit_sphere, norm) > 0.0) {
        return unit_sphere;
    } else {
        return -unit_sphere;
    }
}