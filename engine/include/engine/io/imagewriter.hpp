#ifndef MANA_IMAGEWRITER_HPP
#define MANA_IMAGEWRITER_HPP

#include <string>

#include "engine/common/imagebuffer.hpp"

namespace mana {
    class ImageWriter {
    public:
        static void write(std::string filepath, const ImageBuffer<ColorRGB24> &buffer);

        static void write(std::string filepath, const ImageBuffer<ColorRGBA32> &buffer);
    };
}

#endif //MANA_IMAGEWRITER_HPP
