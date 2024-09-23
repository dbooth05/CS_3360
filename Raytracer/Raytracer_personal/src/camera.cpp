#include "camera.hpp"

void camera::render(const hittable &world) {

    init();

    std::cout << "P6\n" << img_wd << " " << img_ht << "\n255\n";

    for (int j = 0; j < img_ht; j++) {
        std::clog << "\rProgress: " << j << "/" << img_ht << std::flush;
        for (int i = 0; i < img_wd; i++) {
            
            color pix_col(0, 0, 0);
            for (int k = 0; k < anti_alias; k++) {
                ray r = get_ray(i, j);
                pix_col += ray_color(r, world); 
            }

            color output_col = pix_col * anti_alias_scale;

            write_color(std::cout, output_col);
        }
    }

    std::clog << "\rDone.                   \n";
}

void camera::init() {
    
    img_ht = int(img_wd / aspect) < 1 ? 1 : int(img_wd / aspect);

    anti_alias_scale = 1.0 / anti_alias;

    center = lk_from;

    // viewport dims
    auto focal_len = 1.0;
    auto vp_ht = 2.0 * std::tan(deg_to_rad(fov) / 2) * focus_dist;
    auto vp_wd = vp_ht * (double(img_wd) / img_ht);
    cam_center = vec3(0, 0, 0);

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

color camera::ray_color(const ray &r, const hittable &world) {
    hit_record rec;
    if (world.hit(r, interval(0, inf), rec)) {
        return 0.5 * (rec.norm + color(1, 1, 1));
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color (0.5, 0.7, 1.0);
}

ray camera::get_ray(int i, int j) const {
    auto offset = sample_sqr();
    auto pix_sample = pix_loc
                + ((i + offset.x()) * pix_delt_u)
                + ((j + offset.y()) * pix_delt_v);
    auto ray_orig = center;
    auto ray_dir = pix_sample - ray_orig;

    return ray(ray_orig, ray_dir);
}

vec3 camera::sample_sqr() const {
    return vec3(rand_double() - 0.5, rand_double() - 0.5, 0);
}