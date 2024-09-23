#include "color.hpp"

double lin_to_gamma(double lin_comp) {
    if (lin_comp > 0) {
        return std::sqrt(lin_comp);
    }

    return 0;
}

void write_color(std::ostream &out, color &col) {

    static const interval intens(0.000, 0.999);

    auto r = lin_to_gamma(col.x());
    auto g = lin_to_gamma(col.y());
    auto b = lin_to_gamma(col.z());

    auto r_bt = int(255.999 * intens.clamp(r));
    auto g_bt = int(255.999 * intens.clamp(g));
    auto b_bt = int(255.999 * intens.clamp(b));

    out.put(static_cast<unsigned char>(r_bt))
       .put(static_cast<unsigned char>(g_bt))
       .put(static_cast<unsigned char>(b_bt));
}
