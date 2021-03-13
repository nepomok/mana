#include "engine/io/imageloader.hpp"

#include <stdexcept>

#include "../extern/stb_image.h"

namespace mana {
    ImageBuffer<ColorRGBA32> ImageLoader::load(std::string filepath) {
        ImageBuffer<ColorRGBA32> ret;
        int width, height, nrChannels;
        stbi_uc *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            auto *dataCopy = new stbi_uc[sizeof(ColorRGBA32) * width * height];
            std::memcpy(dataCopy, data, sizeof(ColorRGBA32) * width * height);
            stbi_image_free(data);
            Buffer<ColorRGBA32> buffer = Buffer<ColorRGBA32>(width * height,
                                                             reinterpret_cast<ColorRGBA32 *>(dataCopy));
            return ImageBuffer<ColorRGBA32>(width, height, buffer);
        } else {
            stbi_image_free(data);
            std::string error = "Failed to load image ";
            error.append(filepath);
            throw std::runtime_error(error);
        }
    }
}