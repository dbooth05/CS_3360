#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>

#include "constants.hpp"
#include "hittable.hpp"
#include "materials.hpp"

class camera {
    public:
        int img_wd = default_width;
        double aspect = default_aspect;
        int max_depth = default_max_depth;
        int anti_alias = default_anti_alias;

        double fov = default_fov;
        vec3 lk_from = vec3(0, 0, 0);
        vec3 lk_at = vec3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;  // variation angle of rays through each pixel
        double focus_dist    = 10; // distance from cam to lookfrom point to plane of perfect focus

        void render(const hittable &world);

        color ray_color(const ray &r, int depth, const hittable &world);

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

        vec3 defocus_disk_sample() const ;
};

#endif