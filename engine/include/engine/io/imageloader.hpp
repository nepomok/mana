#ifndef MANA_IMAGELOADER_HPP
#define MANA_IMAGELOADER_HPP

#include "engine/common/imagebuffer.hpp"

namespace mana {
    class ImageLoader {
    public:
        static ImageBuffer<ColorRGBA32> load(std::string filepath);
    };
}

#endif //MANA_IMAGELOADER_HPP
