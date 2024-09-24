#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
    public:
        ray() {}

        ray(const vec3 &origin, const vec3 &direction) : orig(origin), dir(direction), tm(0) {}

        ray(const vec3 &origin, const vec3 &direction, double time) : orig(origin), dir(direction), tm(time) {}

        const vec3& origin() const  { return orig; }
        const vec3& direction() const { return dir; }
        
        double time() const { return tm; }

        vec3 at(double t) const { return orig + t*dir; }
        
    private:
        vec3 orig;
        vec3 dir;
        double tm;
};

#endif