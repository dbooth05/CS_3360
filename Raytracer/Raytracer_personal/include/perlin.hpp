#ifndef PERLIN_HPP
#define PERLIN_HPP

#include "vec3.hpp"

inline double rand_double_perlin(int min, int max) {
    return std::rand() / (RAND_MAX + 1.0);
}

inline int rand_int_perlin(int min, int max) {
    return int(rand_double_perlin(min, max+1));
}

class perlin {
    public:
        perlin();

        double noise(const vec3 &p) const;

        double turbulence(const vec3 &p, int depth) const;
    
    private:
        static const int pnt_cnt = 256;
        vec3 rand_vec[pnt_cnt];
        int perm_x[pnt_cnt];
        int perm_y[pnt_cnt];
        int perm_z[pnt_cnt];

        static void generate_perlin(int *p);

        static void permute(int *p, int n);

        static double perlin_interp(const vec3 c[2][2][2], double u, double v, double w);
};

#endif