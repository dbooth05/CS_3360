#ifndef VEC3_HPP
#define VEC3_HPP

#include <iostream>

class vec3 {
    public:
        double e[3];

        vec3();
        vec3(double e0, double e1, double e2);

        double x() const;
        double y() const;
        double z() const;

        vec3 operator-() const;
        double operator[](int i) const;
        double &operator[](int i);

        vec3 &operator+=(const vec3 &v);
        vec3 &operator*=(double t);
        vec3 &operator/=(double t);

        double len() const;
        double len_sqrd() const;
};

using pnt3 = vec3;

// vec utility funcs
inline std::ostream &operator<<(std::ostream &out, const vec3 &v);
inline vec3 operator+(const vec3 &u, const vec3 &v);
inline vec3 operator-(const vec3 &u, const vec3 &v);
inline vec3 operator*(const vec3 &u, const vec3 &v);
inline vec3 operator*(double t, const vec3 &v);
inline vec3 operator*(const vec3 &v, double t);
inline vec3 operator/(const vec3 &v, double t);
inline double dot(const vec3 &u, const vec3 &v);
inline vec3 cross(const vec3 &u, const vec3 &v);
inline vec3 unit_vector(const vec3 &v);

#endif