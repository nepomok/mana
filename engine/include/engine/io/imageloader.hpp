#ifndef MANA_IMAGELOADER_HPP
#define MANA_IMAGELOADER_HPP

#include "engine/render/imagebuffer.hpp"

namespace mana {
    class ImageLoader {
    public:
        static ImageBuffer<ColorRGBA> load(std::string filepath);
    };
}

#endif //MANA_IMAGELOADER_HPP
