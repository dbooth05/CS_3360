#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <sstream>
#include <fstream>
#include <atomic>

#include "pdf.hpp"
#include "constants.hpp"
#include "hittable.hpp"
#include "materials.hpp"
#include "thread_pools.hpp"

color toneMap(const color &hdrcolor) {
    double exposure = 1.0;
    float white = 10.0;
    color mapped = hdrcolor * exposure / (hdrcolor + exposure);
    mapped /= white;
    return mapped;
}

class camera {
    public:
        int img_wd = default_width;
        int img_ht;
        double aspect = default_aspect;
        int max_depth = default_max_depth;
        int anti_alias = default_anti_alias;
        color bg = color(0.0, 1.0, 1.0);
        shared_ptr<texture> bg_tex = nullptr;

        double fov = default_fov;
        vec3 lk_from = vec3(0, 0, 0);
        vec3 lk_at = vec3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);

        double defocus_angle = 0;  // variation angle of rays through each pixel
        double focus_dist    = 10; // distance from cam to lookfrom point to plane of perfect focus

        double gamma = 1.0;
        bool is_hdr = false;

        /*
            Render row function including importance sampling. Rounded pixels :)
        */
        void render_row(int j, const hittable &world, const hittable &lights, std::string** row_output) {

            for (int i = 0; i < img_wd; i++) {
                color pix_col(0, 0, 0);
                for (int k = -sqrt_spp; k <= sqrt_spp; ++k) {
                    for (int s = -sqrt_spp; s <= sqrt_spp; ++s) {
                        if (k * k + s * s <= sqrt_spp * sqrt_spp) {
                            ray r = get_ray(i, j, k, s);
                            pix_col += ray_color(r, max_depth, world, lights);
                        }
                    }
                }

                color out_col = pix_col * anti_alias_scale;
                row_output[j][i] = write_color(out_col, anti_alias, is_hdr, gamma);
            }
        }

        /*
            Render row function not intended for importance sampling. Rounded pixels :)
        */
        void render_row(int j, const hittable &world, std::string** row_output) {

            for (int i = 0; i < img_wd; i++) {
                color pix_col(0, 0, 0);
                for (int k = -sqrt_spp; k <= sqrt_spp; k++) {
                    for (int l = -sqrt_spp; l <= sqrt_spp; l++) {
                        if (k * k + l * l <= sqrt_spp * sqrt_spp) {
                            ray r = get_ray(i, j, k, l);
                            pix_col += ray_color(r, max_depth, world);
                        }
                    }
                }

                color out_col = pix_col * anti_alias_scale;
                row_output[j][i] = write_color(out_col, anti_alias, is_hdr, gamma);
            }
        }

        /*
            Rendering function for when importance sampling is occuring
        */
        void render(const hittable &world, const hittable &lights) {

            init();

            ThreadPool pool(thread::hardware_concurrency());
            
            std::ofstream file("img.ppm");
            file << "P3\n" << img_wd << " " << img_ht << "\n255\n";

            std::string** output = new std::string*[img_ht];
            for (int i = 0; i < img_ht; i++) {
                output[i] = new std::string[img_wd];
            }

            for (int j = 0; j < img_ht; j++) {
                int row = j;
                pool.enqueue(([this, &world, &lights, output, row]() {
                    render_row(row, world, lights, output);
                }));
            }

            pool.wait_till_done();

            for (int i = 0; i < img_ht; i++) {
                for (int j = 0; j < img_wd; j++) {
                    file << output[i][j];
                }
            }
        }

        /*
            Rendering function when no importance sampling being used
        */
        void render(const hittable &world) {

            init();

            // ThreadPool pool(thread::hardware_concurrency());
            ThreadPool pool(1);

            std::ofstream file("img.ppm");
            file << "P3\n" << img_wd << " " << img_ht << "\n255\n";

            std::string** output = new std::string*[img_ht];
            for (int i = 0; i < img_ht; i++) {
                output[i] = new std::string[img_wd];
            }

            for (int j = 0; j < img_ht; j++) {
                int row = j;
                pool.enqueue(([this, &world, output, row]() {
                    render_row(row, world, output);
                }));
            }

            pool.wait_till_done();

            for (int i = 0; i < img_ht; i++) {
                for (int j = 0; j < img_wd; j++) {
                    file << output[i][j];
                }
            }

        }

        /*
            Gets ray color while using importance sampling
        */
        color ray_color(const ray &r, int depth, const hittable &world, const hittable &lights) {
            if (depth <= 0) { return color(0, 0, 0); }

            hit_record rec;

            if (!world.hit(r, interval(0.001, inf), rec)) {
                return bg;
            }

            scatter_record srec;
            
            color col_from_emission = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, srec)) {
                return col_from_emission;
            }

            if (srec.skip_pdf) {
                return srec.atten * ray_color(srec.skip_pdf_ray, depth-1, world, lights);
            }

            auto lht_ptr = make_shared<hittable_pdf>(lights, rec.p);
            mix_pdf p(lht_ptr, srec.pdf_ptr);

            ray scattered = ray(rec.p, p.generate(), r.time());
            auto pdf_val = p.value(scattered.direction());

            double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

            color sample_col = ray_color(scattered, depth-1, world, lights);
            color col_from_scatter = (srec.atten * scattering_pdf * sample_col) / pdf_val;

            return col_from_emission + col_from_scatter;
        }

        /*
            Gets ray color when there is NOT importance sampling
        */
        color ray_color(const ray &r, int depth, const hittable &world) {
            if (depth <= 0) { return color(0, 0, 0); }

            hit_record rec;

            if (!world.hit(r, interval(0.001, inf), rec)) {
                if (bg_tex) {
                    auto unit_dir = unit_vector(r.direction());
                    double u, v; get_spherical_uv(unit_dir, u, v);
                    return bg_tex->value(u, v, unit_dir);
                } else {
                    return bg;
                }
            }

            ray scattered;
            color atten;
            color col_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

            if (!rec.mat->scatter(r, rec, atten, scattered)) {
                return col_from_emission;
            }

            color col_from_scatter = atten * ray_color(scattered, depth - 1, world);

            return col_from_emission + col_from_scatter;
        }

    private:
        double anti_alias_scale;

        int sqrt_spp;           // square root of number of samples per pixel
        double recip_sqrt_spp;  // 1 / sqrt_spp

        int progress = 0;

        vec3 center, cam_center, pix_loc;
        vec3 pix_delt_u, pix_delt_v;

        vec3 u, v, w;

        vec3 defocus_u, defocus_v;

        void init() {
            
            img_ht = int(img_wd / aspect) < 1 ? 1 : int(img_wd / aspect);

            sqrt_spp = int(std::sqrt(anti_alias));
            anti_alias_scale = 1.0 / (sqrt_spp * sqrt_spp);
            recip_sqrt_spp = 1.0 / sqrt_spp;

            center = lk_from;

            // viewport dims
            // auto focal_len = (lk_from - lk_at).len();
            auto theta = deg_to_rad(fov);
            auto h = std::tan(theta/2);
            auto vp_ht = 2 * h * focus_dist;
            auto vp_wd = vp_ht * (double(img_wd)/img_ht);

            // calculate basis unit vector
            w = unit_vector(lk_from - lk_at);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // calculate horiz/vert vp edges
            auto vp_u = vp_wd * u;
            auto vp_v = vp_ht * -v;

            // calculate horiz/vert delta vectors between pixels
            pix_delt_u = vp_u / img_wd;
            pix_delt_v = vp_v / img_ht;

            // calculate upper left pixel location
            auto vp_ul = center - (focus_dist * w) - vp_u / 2 - vp_v / 2;
            pix_loc = vp_ul + 0.5 * (pix_delt_u + pix_delt_v);

            // calculate camera defocus basis vectors
            auto defocus_rad = focus_dist * std::tan(deg_to_rad(defocus_angle / 2));
            defocus_u = u * defocus_rad;
            defocus_v = v * defocus_rad;
        }

        /*
            Get ray for rounded pixels (sphere anti-aliasing)
        */
        ray get_ray(int i, int j, int s_i, int s_j) const {
            auto offset = sample_sqr_stratified(s_i, s_j);
            auto pix_sample = pix_loc
                        + ((i + offset.x()) * pix_delt_u)
                        + ((j + offset.y()) * pix_delt_v);
            auto ray_orig = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_dir = pix_sample - ray_orig;

            auto r_time = random_double();

            return ray(ray_orig, ray_dir, r_time);
        }

        /*
            Get ray for square anti-aliasing
        */
        ray get_ray(int i, int j) const {
            auto offset = sample_sqr();
            auto pix_sample = pix_loc
                        + ((i + offset.x()) * pix_delt_u)
                        + ((j + offset.y()) * pix_delt_v);
            auto ray_orig = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_dir = pix_sample - ray_orig;

            auto r_time = random_double();

            return ray(ray_orig, ray_dir, r_time);
        }

        vec3 sample_sqr_stratified(int s_i, int s_j) const {
            auto px = ((s_i + rand_double()) * recip_sqrt_spp) - 0.5;
            auto py = ((s_j + rand_double()) * recip_sqrt_spp) - 0.5;

            return vec3(px, py, 0);
        }

        vec3 sample_sqr() const {
            return vec3(rand_double() - 0.5, rand_double() - 0.5, 0);
        }

        vec3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_u) + (p[1] * defocus_v);
        }
};

#endif