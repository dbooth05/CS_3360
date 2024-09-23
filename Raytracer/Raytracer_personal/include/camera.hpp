#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "constants.hpp"
#include "hittable.hpp"

class camera {
    public:
        int img_wd = DEFAULT_WIDTH;
        double aspect = DEFAULT_ASPECT;
        int max_depth = DEFAULT_MAX_DEPTH;
        int anti_alias = DEFAULT_ANTI_ALIAS;

        double fov = DEFAULT_FOV;
        vec3 lk_from = vec3(0, 0, 0);
        vec3 lk_at = vec3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;  // variation angle of rays through each pixel
        double focus_dist    = 10; // distance from cam to lookfrom point to plane of perfect focus

        void render(const hittable &world);

        color ray_color(const ray &r, const hittable &world);

    private:
        int img_ht;
        double anti_alias_scale;

        vec3 center, cam_center, pix_loc;
        vec3 pix_delt_u, pix_delt_v;

        vec3 u, v, w;

        vec3 defocus_u, defocus_v;

        void init();

        ray get_ray(int i, int j) const;

        vec3 sample_sqr() const;
};

#endif