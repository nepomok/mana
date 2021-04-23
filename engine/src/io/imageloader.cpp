#include "engine/io/imageloader.hpp"

#include <stdexcept>

#include "../extern/stb_image.h"

namespace mana {
    ImageBuffer<ColorRGBA> ImageLoader::load(std::string filepath) {
        ImageBuffer<ColorRGBA> ret;
        int width, height, nrChannels;
        stbi_uc *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            auto *dataCopy = new stbi_uc[sizeof(ColorRGBA) * width * height];
            std::memcpy(dataCopy, data, sizeof(ColorRGBA) * width * height);
            stbi_image_free(data);
            Buffer<ColorRGBA> buffer = Buffer<ColorRGBA>(width * height,
                                                         reinterpret_cast<ColorRGBA *>(dataCopy));
            return ImageBuffer<ColorRGBA>(width, height, buffer);
        } else {
            stbi_image_free(data);
            std::string error = "Failed to load image ";
            error.append(filepath);
            throw std::runtime_error(error);
        }
    }
}