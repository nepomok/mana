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

#include "engine/resource/render/texturebufferresource.hpp"

namespace mana {
    TextureBufferResource::TextureBufferResource()
            : renderDevice(nullptr),
              image(),
              manager(nullptr),
              attrib(),
              texture(nullptr) {}

    TextureBufferResource::TextureBufferResource(RenderDevice &device,
                                                 std::string image,
                                                 ResourceManager *manager,
                                                 TextureBuffer::Attributes attributes)
            : renderDevice(&device),
              image(std::move(image)),
              manager(manager),
              attrib(attributes),
              texture(nullptr) {}

    TextureBufferResource::~TextureBufferResource() {
        TextureBufferResource::free();
    }

    void TextureBufferResource::load() {
        if (loaded)
            return;
        manager->incrementRef(image);
        auto *img = manager->getResource<Image<ColorRGBA>>(image);
        attrib.size = img->get().getSize();
        if (attrib.textureType == TextureBuffer::TEXTURE_CUBE_MAP)
            attrib.size.x = attrib.size.x / 6;
        texture = renderDevice->getAllocator().createTextureBuffer(attrib);
        if (attrib.textureType == TextureBuffer::TEXTURE_CUBE_MAP) {
            texture->uploadCubeMap(img->get());
        } else {
            texture->upload(img->get());
        }
        loaded = true;
    }

    void TextureBufferResource::free() {
        if (!loaded)
            return;
        delete texture;
        texture = nullptr;
        manager->decrementRef(image);
        loaded = false;
    }

    TextureBuffer &TextureBufferResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return *texture;
    }

    bool TextureBufferResource::isLoaded() {
        return loaded;
    }

    bool TextureBufferResource::supportAsync() {
        return false;
    }

    TextureBuffer &TextureBufferResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}