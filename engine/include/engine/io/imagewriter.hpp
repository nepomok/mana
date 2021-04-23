#ifndef MANA_IMAGEWRITER_HPP
#define MANA_IMAGEWRITER_HPP

#include <string>

#include "engine/render/imagebuffer.hpp"

namespace mana {
    class ImageWriter {
    public:
        static void write(std::string filepath, const ImageBuffer<ColorRGB> &buffer);

        static void write(std::string filepath, const ImageBuffer<ColorRGBA> &buffer);
    };
}

#endif //MANA_IMAGEWRITER_HPP
