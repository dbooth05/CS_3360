#include "color.hpp"

void write_color(std::ostream &out, color &col) {

    static const interval intens(0.000, 0.999);

    auto r = col.x();
    auto g = col.y();
    auto b = col.z();

    auto r_bt = int(255.999 * intens.clamp(r));
    auto g_bt = int(255.999 * intens.clamp(g));
    auto b_bt = int(255.999 * intens.clamp(b));

    out.put(static_cast<unsigned char>(r_bt))
       .put(static_cast<unsigned char>(g_bt))
       .put(static_cast<unsigned char>(b_bt));
}
