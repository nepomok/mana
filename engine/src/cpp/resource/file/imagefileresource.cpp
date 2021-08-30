/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "engine/resource/file/imagefileresource.hpp"

#include "extern/stb_image.h"

#include <cstring>

namespace mana {
    Image<ColorRGBA> readImageFile_(const std::string &filePath) {
        int width, height, nrChannels;
        stbi_uc *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            auto ret = Image<ColorRGBA>(width, height);
            std::memcpy(ret.getData(), data, (width * height) * (sizeof(stbi_uc) * 4));
            stbi_image_free(data);
            return ret;
        } else {
            stbi_image_free(data);
            std::string error = "Failed to load image ";
            error.append(filePath);
            throw std::runtime_error(error);
        }
    }

    ImageFileResource::ImageFileResource()
            : filePath() {}

    ImageFileResource::ImageFileResource(std::string filePath)
            : filePath(std::move(filePath)) {}

    ImageFileResource::~ImageFileResource() {
        ImageFileResource::free();
    }

    void ImageFileResource::load() {
        if (loaded)
            return;
        buffer = readImageFile_(filePath);
        loaded = true;
    }

    void ImageFileResource::free() {
        if (!loaded)
            return;
        buffer = {};
        loaded = false;
    }

    Image<ColorRGBA> &ImageFileResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return buffer;
    }

    bool ImageFileResource::isLoaded() {
        return loaded;
    }

    bool ImageFileResource::supportAsync() {
        return false;
    }

    Image<ColorRGBA> &ImageFileResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}
