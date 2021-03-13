#include "engine/io/imagewriter.hpp"

#include "../extern/stb_image.h"

namespace mana {
    void ImageWriter::write(std::string filepath, const ImageBuffer<ColorRGB24> &buffer) {
    }

    void ImageWriter::write(std::string filepath, const ImageBuffer<ColorRGBA32> &buffer) {
    }
}