#include "color.hpp"

void write_color(std::ostream &out, color &col) {
    auto r_bt = int(255.999 * col.e[R]);
    auto g_bt = int(255.999 * col.e[G]);
    auto b_bt = int(255.999 * col.e[B]);

    out.put(static_cast<unsigned char>(r_bt))
       .put(static_cast<unsigned char>(g_bt))
       .put(static_cast<unsigned char>(b_bt));
}
