#include "camera.hpp"

void camera::render_row(int j, const hittable &world, std::string** row_output) { 

    for (int i = 0; i < img_wd; i++) {
        color pix_col(0, 0, 0);
        for (int k = 0; k < anti_alias; k++) {
            ray r = get_ray(i, j);
            pix_col += ray_color(r, max_depth, world);
        }

        color out_col = pix_col * anti_alias_scale;
        row_output[j][i] = write_color(out_col);
    }
}

void camera::render(const hittable &world) {

    init();

    ThreadPool pool(thread::hardware_concurrency());
    
    progress = 0;

    std::cout << "P3\n" << img_wd << " " << img_ht << "\n255\n";

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
            std::cout << output[i][j];
        }
    }
}

void camera::init() {
    
    img_ht = int(img_wd / aspect) < 1 ? 1 : int(img_wd / aspect);

    anti_alias_scale = 1.0 / anti_alias;

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

color camera::ray_color(const ray &r, int depth, const hittable &world) {

    if (depth <= 0) { return color(0, 0, 0); }

    hit_record rec;

    if (!world.hit(r, interval(0.001, inf), rec)) {
        return bg;
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

ray camera::get_ray(int i, int j) const {
    auto offset = sample_sqr();
    auto pix_sample = pix_loc
                + ((i + offset.x()) * pix_delt_u)
                + ((j + offset.y()) * pix_delt_v);
    auto ray_orig = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_dir = pix_sample - ray_orig;

    auto r_time = random_double();

    return ray(ray_orig, ray_dir, r_time);
}

vec3 camera::sample_sqr() const {
    return vec3(rand_double() - 0.5, rand_double() - 0.5, 0);
}

vec3 camera::defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_u) + (p[1] * defocus_v);
}