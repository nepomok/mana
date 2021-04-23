#include "engine/io/imagewriter.hpp"

#include "../extern/stb_image.h"

namespace mana {
    void ImageWriter::write(std::string filepath, const ImageBuffer<ColorRGB> &buffer) {
    }

    void ImageWriter::write(std::string filepath, const ImageBuffer<ColorRGBA> &buffer) {
    }
}