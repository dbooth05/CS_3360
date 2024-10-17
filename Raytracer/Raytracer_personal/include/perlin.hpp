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
        perlin() {
            for (int i = 0; i < pnt_cnt; i++) {
                rand_vec[i] = unit_vector(vec3::random(-1, 1));
            }

            perlin::generate_perlin(perm_x);
            perlin::generate_perlin(perm_y);
            perlin::generate_perlin(perm_z);
        }

        double noise(const vec3 &p) const {
            auto u = p.x() - std::floor(p.x());
            auto v = p.y() - std::floor(p.y());
            auto w = p.z() - std::floor(p.z());

            auto i = int(std::floor(p.x()));
            auto j = int(std::floor(p.y()));
            auto k = int(std::floor(p.z()));
            vec3 c[2][2][2];

            for (int di=0; di < 2; di++) {
                for (int dj=0; dj < 2; dj++) {
                    for (int dk=0; dk < 2; dk++) {
                        c[di][dj][dk] = rand_vec[
                            perm_x[(i+di) & 255] ^
                            perm_y[(j+dj) & 255] ^
                            perm_z[(k+dk) & 255]
                        ];
                    }
                }
            }

            return perlin_interp(c, u, v, w);
        }

        double turbulence(const vec3 &p, int depth) const {
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(temp_p);
                weight /= 2;
                temp_p *= 2;
            }

            return std::fabs(accum);
        }
    
    private:
        static const int pnt_cnt = 256;
        vec3 rand_vec[pnt_cnt];
        int perm_x[pnt_cnt];
        int perm_y[pnt_cnt];
        int perm_z[pnt_cnt];

        static void generate_perlin(int *p) {
            for(int i = 0; i < pnt_cnt; i++) {
                p[i] = i;
            }

            permute(p, pnt_cnt);
        }

        static void permute(int *p, int n) {
            for (int i = n-1; i > 0; i--) {
                int t = rand_int_perlin(0, i);
                int tmp = p[i];
                p[i] = p[t];
                p[t] = tmp;
            }
        }

        static double perlin_interp(const vec3 c[2][2][2], double u, double v, double w) {
            auto uu = u*u*(3-2*u);
            auto vv = v*v*(3-2*v);
            auto ww = w*w*(3-2*w);
            auto accum = 0.0;

            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++) {
                        vec3 weight_v(u-i, v-j, w-k);
                        accum += (i*uu + (1-i)*(1-uu))
                                * (j*vv + (1-j)*(1-vv))
                                * (k*ww + (1-k)*(1-ww))
                                * dot(c[i][j][k], weight_v);
                    }

            return accum;
        }
};

#endif