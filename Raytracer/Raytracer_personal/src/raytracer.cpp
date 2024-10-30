#include <string>

#include "camera.hpp"
#include "constants.hpp"
#include "materials.hpp"
#include "bvh.hpp"
#include "mesh_loader.hpp"
#include "mediums.hpp"

enum class scenes {
    CUSTOM,
    BOUNCING_SPHERES,
    CHECKERED_SPHERES,
    PERLIN_SPHERE,
    QUADS,
    LIGHT,
    CORNELL_BOX,
    CORNELL_SMOKE,
    EARTH,
    BOOK2_FINAL,
    TRIANGLE,
    NUM_SCENES
};

enum class camera_settings {
    IMG_WD,
    ASPECT,
    MAX_DEPTH,
    ANTI_ALIAS,
    BACKGROUND_COLOR,
    FOV,
    LK_FROM,
    LK_AT,
    VUP,
    DEFOCUS_ANGLE,
    FOCUS_DIST
};

void* cam_settings[static_cast<int>(camera_settings::FOCUS_DIST)+1];

void my_custom_scene(hittable_list &world, camera &cam) {

    auto mat_grnd = make_shared<lamber>(color(0.098, 0.0, 0.2));
    auto mirror = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto glass = make_shared<dialectric>(1.0 / 1.5);
    auto glass1 = make_shared<dialectric>(1.5);
    auto light = make_shared<diffuse_light>(color(10, 10, 10));
    auto white = make_shared<lamber>(color(0.99, 0.99, 0.99));

    world.add(make_shared<sphere>(vec3(0, -1001, 0), 1000, mat_grnd));
    world.add(make_shared<sphere>(vec3(-1.25, 0, 0), 1.0, glass1));
    world.add(make_shared<sphere>(vec3(-1.25, 0, 0), 0.5, glass));
    world.add(make_shared<sphere>(vec3( 1.25, 0, 0), 1.0, mirror));
    world.add(make_shared<sphere>(vec3(0, 4, 0), 1.5, light));

    auto bg_mat = make_shared<lamber>(color(1.0, 0.0, 0.0));
    for (int i = 0; i < 4; i++) {
        world.add(make_shared<sphere>(vec3(-3 + (i * 2), 0.0, -6), 1.0, bg_mat));
    }

    shared_ptr<hittable> cloud = make_shared<sphere>(vec3(0, 4, 0), 3, white);
    world.add(make_shared<medium>(cloud, 0.001, color(0.8, 0.8, 0.8)));

    cam.img_wd = 1900;
    cam.aspect = 16.0 / 9.0;
    cam.anti_alias = 1000;
    cam.max_depth = 500;
    cam.lk_from = vec3(0, 0, 10);

    cam.defocus_angle = 0;

    cam.bg = color(0, 0, 0);
}

void bouncing_spheres(hittable_list &world) {

    auto mat_grnd = make_shared<lamber>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, mat_grnd));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).len() > 0.9) {
                shared_ptr<material> sphere_mat;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_mat = make_shared<lamber>(albedo);
                    auto c2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, c2, 0.2, sphere_mat));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_mat = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                } else {
                    sphere_mat = make_shared<dialectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_mat));
                }
            }
        }
    }

    auto mat1 = make_shared<dialectric>(1.5);
    auto mat2 = make_shared<lamber>(color(0.4, 0.2, 0.1));
    auto mat3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, mat1));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, mat2));
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, mat3));

    world = hittable_list(make_shared<bvh_node>(world));
}

void checkered_spheres(hittable_list &world) {
    
    auto checker = make_shared<checkers>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lamber>(checker)));
    world.add(make_shared<sphere>(vec3(0,  10, 0), 10, make_shared<lamber>(checker)));

}

void perlin_sphere(hittable_list &world, camera &cam) {
    auto perlin = make_shared<noise_tex>(4);
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lamber>(perlin)));
    world.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lamber>(perlin)));

    cam.defocus_angle = 0;
}

void quads(hittable_list &world, camera &cam) {
    auto red = make_shared<lamber>(color(1.0, 0.2, 0.2));
    auto grn = make_shared<lamber>(color(0.2, 1.0, 0.2));
    auto blu = make_shared<lamber>(color(0.2, 0.2, 1.0));
    auto org = make_shared<lamber>(color(1.0, 0.5, 0.0));
    auto tel = make_shared<lamber>(color(0.2, 0.8, 0.8));

    world.add(make_shared<quad>(vec3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), red));
    world.add(make_shared<quad>(vec3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), grn));
    world.add(make_shared<quad>(vec3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), blu));
    world.add(make_shared<quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), org));
    world.add(make_shared<quad>(vec3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), tel));

    cam.aspect = 1.0;
    cam.img_wd = 400;
    cam.anti_alias = 1000;
    cam.max_depth = 500;

    cam.fov = 80;
    cam.lk_from = vec3(0, 0, 9);
    cam.lk_at = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void light(hittable_list &world, camera &cam) {
    auto pertext = make_shared<noise_tex>(4);
    world.add(make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lamber>(pertext)));
    world.add(make_shared<sphere>(vec3(0, 2, 0), 2, make_shared<lamber>(pertext)));

    auto diff_light = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<sphere>(vec3(0, 7, 0), 2, diff_light));
    world.add(make_shared<quad>(vec3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), diff_light));

    cam.fov = 20;
    cam.lk_from = vec3(26, 3, 6);
    cam.lk_at = vec3(0, 2, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.anti_alias = 200;
    cam.max_depth = 250;

    cam.bg = color(0, 0, 0);
}

void cornell_box(hittable_list &world, camera &cam) {

    auto red   = make_shared<lamber>(color(.65, .05, .05));
    auto wht = make_shared<lamber>(color(.73, .73, .73));
    auto grn = make_shared<lamber>(color(.12, .45, .15));
    auto lht = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(vec3(555,0,0), vec3(0,555,0), vec3(0,0,555), grn));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(vec3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), lht));
    world.add(make_shared<quad>(vec3(0,0,0), vec3(555,0,0), vec3(0,0,555), wht));
    world.add(make_shared<quad>(vec3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), wht));
    world.add(make_shared<quad>(vec3(0,0,555), vec3(555,0,0), vec3(0,555,0), wht));

    shared_ptr<hittable> b1 = box(vec3(0, 0, 0), vec3(165, 330, 165), wht);
    b1 = make_shared<rotate_y>(b1, 15);
    b1 = make_shared<translate>(b1, vec3(265, 0, 295));
    world.add(b1);

    shared_ptr<hittable> b2 = box(vec3(0, 0, 0), vec3(165, 165, 165), wht);
    b2 = make_shared<rotate_y>(b2, -18);
    b2 = make_shared<translate>(b2, vec3(130, 0, 65));
    world.add(b2);

    cam.aspect = 1.0;
    cam.img_wd = 600;
    cam.anti_alias = 100;
    cam.max_depth = 5;
    cam.bg = color(0,0,0);

    cam.fov = 40;
    cam.lk_from = vec3(278, 278, -800);
    cam.lk_at = vec3(278, 278, 0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
}

void cornell_smoke(hittable_list &world, camera &cam) {
    auto red = make_shared<lamber>(color(0.65, 0.05, 0.05));
    auto wht = make_shared<lamber>(color(0.73, 0.73, 0.73));
    auto grn = make_shared<lamber>(color(0.12, 0.45, 0.15));
    auto lgt = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(vec3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), grn));
    world.add(make_shared<quad>(vec3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(make_shared<quad>(vec3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), lgt));
    world.add(make_shared<quad>(vec3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), wht));
    world.add(make_shared<quad>(vec3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), wht));
    world.add(make_shared<quad>(vec3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), wht));

    // two small boxes
    shared_ptr<hittable> b1 = box(vec3(0, 0, 0), vec3(165, 330, 165), wht);
    b1 = make_shared<rotate_y>(b1, 15);
    b1 = make_shared<translate>(b1, vec3(265, 0, 295));
    world.add(make_shared<medium>(b1, 0.01, color(0, 0, 0)));

    shared_ptr<hittable> b2 = box(vec3(0, 0, 0), vec3(165, 165, 165), wht);
    b2 = make_shared<rotate_y>(b2, -18);
    b2 = make_shared<translate>(b2, vec3(140, 0, 65));
    world.add(make_shared<medium>(b2, 0.01, color(1, 1, 1)));

    cam.aspect = 1.0;
    cam.img_wd = 400;
    cam.anti_alias = 100;
    cam.max_depth = 50;
    cam.bg = color(0, 0, 0);

    cam.fov = 40;
    cam.lk_from = vec3(278, 278, -800);
    cam.lk_at = vec3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void earth(hittable_list &world, camera &cam) {

    auto earth_tex = make_shared<image_tex>("earthmap.jpg");
    auto earth_sur = make_shared<lamber>(earth_tex);
    auto globe = make_shared<sphere>(vec3(0, 0, 0), 2, earth_sur);

    world.add(globe);

    cam.img_wd = 400;
    cam.anti_alias = 100;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lk_from = vec3(0, 0, 12);
    cam.lk_at = vec3(0, 0, 0);
    cam.defocus_angle = 0;
}

void book2_final(hittable_list &world, camera &cam) {
    
    hittable_list boxes;
    auto gnd = make_shared<lamber>(color(0.48, 0.83, 0.53));

    int box_cnt = 20;
    for (int i = 0; i < box_cnt; i++) {
        for (int j = 0; j < box_cnt; j++) {
            auto w = 100.0;
            auto x_0 = -1000.0 + i * w;
            auto z_0 = -1000.0 + j * w;
            auto y_0 = 0.0;
            auto x_1 = x_0 + w;
            auto y_1 = random_double(1, 101);
            auto z_1 = z_0 + w;

            boxes.add(box(vec3(x_0, y_0, z_0), vec3(x_1, y_1, z_1), gnd));
        }
    }

    world.add(make_shared<bvh_node>(boxes));

    auto lght = make_shared<diffuse_light>(color(7, 7, 7));
    world.add(make_shared<quad>(vec3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), lght));

    auto c1 = vec3(400, 400, 200);
    auto c2 = c1 + vec3(30, 0, 0);
    auto sphere_mat = make_shared<lamber>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(c1, c2, 50, sphere_mat));

    world.add(make_shared<sphere>(vec3(260, 150, 45), 50, make_shared<dialectric>(1.5)));
    world.add(make_shared<sphere>(vec3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<sphere>(vec3(360, 150, 145), 70, make_shared<dialectric>(1.5));
    world.add(boundary);
    world.add(make_shared<medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(vec3(0, 0, 0), 5000, make_shared<dialectric>(1.5));
    world.add(make_shared<medium>(boundary, 0.0001, color(1, 1, 1)));

    auto emat = make_shared<lamber>(make_shared<image_tex>("earthmap.jpg"));
    world.add(make_shared<sphere>(vec3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_tex>(0.2);
    world.add(make_shared<sphere>(vec3(220, 280, 300), 80, make_shared<lamber>(pertext)));

    hittable_list boxes2;
    auto wht = make_shared<lamber>(color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(vec3::random(0, 165), 10, wht));
    }

    world.add(make_shared<translate>(make_shared<rotate_y>(make_shared<bvh_node>(boxes2), 15), vec3(-100, 270, 395)));

    cam.aspect = 1.0;
    cam.img_wd = 1000;
    cam.anti_alias = 10000;
    cam.max_depth = 20;
    cam.bg = color(0, 0, 0);

    cam.fov = 40;
    cam.lk_from = vec3(478, 278, -600);
    cam.lk_at = vec3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;
}

void triangle_scene(hittable_list &world, camera &cam) {

    auto red = make_shared<lamber>(color(1.0, 0.0, 0.0));

    world.add(make_shared<triangle>(
        vec3(0, 1, 0),
        vec3(-1, 0, 0),
        vec3(1, 0, 0),
        red
    ));

    cam.lk_from = vec3(0, 5, -5);
    cam.lk_at = vec3(0, 0, 0);
}

bool teapot(hittable_list &world, camera &cam) {

    // can be any material type. Could take in a list of files
    // and a list of materials
    auto mat = std::make_shared<lamber>(color(0.9, 0.0, 0.0));

    obj_loader loader;

    if (loader.load("./objects/teapot_no_plane.obj", mat)) {
        std::clog << "loaded " << loader.get_triangles().size() << " triangles\n" << std::flush;
    } else {
        std::cerr << "Failed to load: " << "box.obj" << std::endl;
        return false;
    }

    for (const auto &tri : loader.get_triangles()) {
        world.add(tri);
    }

    // add light to scene
    auto diff_light = make_shared<diffuse_light>(color(8, 8, 8));
    world.add(make_shared<quad>(vec3(-2, 1, -3.75), vec3(5, 0, 0), vec3(0, 2, 0), diff_light));   

    auto mirror = make_shared<metal>(color(0.5, 0.5, 0.5), 0.0);
    auto glass = make_shared<dialectric>(1.0 / 1.5);
    auto g1 = make_shared<dialectric>(1.5);
    world.add(make_shared<sphere>(vec3(-4, 2, 2), 1.5, glass)); 
    world.add(make_shared<sphere>(vec3(-4, 2, 2), 2, g1));
    world.add(make_shared<sphere>(vec3(-4, 2, 2), 0.5, diff_light));

    // The containing box
    // Floor:
    auto gnd = make_shared<metal>(color(1.0, 0.3, 0.3), 0.75);    
    world.add(make_shared<quad>(vec3(-6, 0, -4), vec3(18, 0, 0), vec3(0, 0, 15), gnd));
    // Ceiling:
    auto ceil = make_shared<metal>(color(1.0, 0.3, 0.3), 0.9);
    world.add(make_shared<quad>(vec3(-6, 10, -4), vec3(18, 0, 0), vec3(0, 0, 15), ceil));
    // Back wall
    auto walls = make_shared<lamber>(color(0.098, 0.0, 0.2));
    world.add(make_shared<quad>(vec3(-6, 0, -4), vec3(18, 0, 0), vec3(0, 10, 0), walls));
    // front wall
    world.add(make_shared<quad>(vec3(-6, 0, 11), vec3(18, 0, 0), vec3(0, 10, 0), walls));
    // left wall
    world.add(make_shared<quad>(vec3(-6, 0, -4), vec3(0, 10, 0), vec3(0, 0, 15), walls));
    // right wall
    world.add(make_shared<quad>(vec3(12, 0, -4), vec3(0, 10, 0), vec3(0, 0, 15), walls));

    cam.img_wd = 900;
    cam.anti_alias = 2500;
    cam.max_depth = 250;

    cam.lk_from = vec3(11, 5, 10); // +: right  +: up   +: left?
    cam.lk_at = vec3(0, 1.5, 0); // teapot centered
    cam.vup = vec3(0, 1, 0);

    cam.bg = color(0.0, 0.0, 0.0);

    return true;
}

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();

    hittable_list world;

    camera cam;

    cam.img_wd = 768;
    cam.img_wd = 400;
    cam.aspect = 16.0 / 9.0;
    cam.anti_alias = 50;
    cam.max_depth = 10;

    cam.fov = 20;
    cam.lk_from = vec3(13, 10, 10);
    cam.lk_at = vec3(0, 10, 0);
    cam.vup = vec3(0, 5, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.bg = color(0.70, 0.80, 1.00);

    int select = 11;

    if (argc > 1) {
        for (int i = 0; i < argc; i++) {

            std::string arg = argv[i];

            if (arg.find("-scene=") == 0) {
                select = std::stoi(arg.substr(7));
            } else if (arg.find("-alias=") == 0) {
                cam.anti_alias = std::stoi(arg.substr(7)) > 0 ? std::stoi(arg.substr(7)) : default_anti_alias;
            }
        }
    }

    std::clog << "option: " << select << "\n" << std::flush;

    switch (select)
    {
        case 0: my_custom_scene(world, cam); break;
        case 1: bouncing_spheres(world); break;
        case 2: checkered_spheres(world); break;
        case 3: perlin_sphere(world, cam); break;
        case 4: quads(world, cam); break;
        case 5: light(world, cam); break;
        case 6: cornell_box(world, cam); break;
        case 7: cornell_smoke(world, cam); break;
        case 8: earth(world, cam); break;
        case 9: book2_final(world, cam); break;
        case 10: triangle_scene(world, cam); break;
        case 11: 
            if (!teapot(world, cam)) {
                return false;
            }
            break;
        default:my_custom_scene(world, cam); break;
    }

    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world);

    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::minutes>(end - start);

    std::clog << "\nTime: " << dur.count() << " minutes\n" << std::flush;

    return 0;
}