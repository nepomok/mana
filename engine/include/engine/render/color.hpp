#ifndef MANA_COLOR_HPP
#define MANA_COLOR_HPP

#include <cstdint>

namespace mana {
    struct ColorRGB {
        uint8_t data[3];

        uint8_t &r() { return data[0]; }

        uint8_t &g() { return data[1]; }

        uint8_t &b() { return data[2]; }

        ColorRGB() : data() {}

        ColorRGB(uint8_t r, uint8_t g, uint8_t b)
                : data() {
            this->r() = r;
            this->g() = g;
            this->b() = b;
        }

        explicit ColorRGB(uint8_t v)
                : data() {
            this->r() = v;
            this->g() = v;
            this->b() = v;
        }
    };

    struct ColorRGBA {
        uint8_t data[4];

        uint8_t &r() { return data[0]; }

        uint8_t &g() { return data[1]; }

        uint8_t &b() { return data[2]; }

        uint8_t &a() { return data[3]; }

        ColorRGBA() : data() {}

        ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
                : data() {
            this->r() = r;
            this->g() = g;
            this->b() = b;
            this->a() = a;
        }

        explicit ColorRGBA(uint8_t v)
                : data() {
            this->r() = v;
            this->g() = v;
            this->b() = v;
            this->a() = v;
        }
    };
}

#endif //MANA_COLOR_HPP
