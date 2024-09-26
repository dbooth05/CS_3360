#include "color.hpp"

double lin_to_gamma(double lin_comp) {
    if (lin_comp > 0) {
        return std::sqrt(lin_comp);
    }

    return 0;
}

std::string write_color(color &col) {

    static const interval intens(0.000, 0.999);

    auto r = lin_to_gamma(col.x());
    auto g = lin_to_gamma(col.y());
    auto b = lin_to_gamma(col.z());

    auto r_bt = int(255.999 * intens.clamp(r));
    auto g_bt = int(255.999 * intens.clamp(g));
    auto b_bt = int(255.999 * intens.clamp(b));

    return std::to_string(r_bt) + ' ' + std::to_string(g_bt) + ' '  + std::to_string(b_bt) + '\n';
}

color checkers::value(double u, double v, const vec3 &p) const {
    auto x = int(std::floor(inv_scale * p.x()));
    auto y = int(std::floor(inv_scale * p.y()));
    auto z = int(std::floor(inv_scale * p.z()));

    bool is_even = (x + y + z) % 2 == 0;

    return is_even ? even->value(u, v, p) : odd->value(u, v, p);
}
