#ifndef MANA_COLOR_HPP
#define MANA_COLOR_HPP

#include <cstdint>

namespace mana {
    enum ColorFormat {
        RGB24,
        RGBA32
    };

    struct ColorRGB24 {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        ColorRGB24() : r(0), g(0), b(0) {}

        ColorRGB24(uint8_t r, uint8_t g, uint8_t b)
                : r(r), g(g), b(b) {}

        ColorRGB24(uint8_t v)
                : r(v), g(v), b(v) {}
    };

    struct ColorRGBA32 {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        ColorRGBA32() : r(0), g(0), b(0), a(0) {}

        ColorRGBA32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
                : r(r), g(g), b(b), a(a) {}

        ColorRGBA32(uint8_t v)
                : r(v), g(v), b(v), a(v) {}
    };
}

#endif //MANA_COLOR_HPP
