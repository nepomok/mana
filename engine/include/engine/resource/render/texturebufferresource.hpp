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

#ifndef MANA_TEXTUREBUFFERRESOURCE_HPP
#define MANA_TEXTUREBUFFERRESOURCE_HPP

#include "engine/resource/imageresource.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    class TextureBufferResource : public Resource {
    public:
        TextureBufferResource();

        TextureBufferResource(RenderAllocator &allocator, ImageResource &resource, TextureBuffer::Attributes attributes);

        ~TextureBufferResource() override;

        void load() override;

        void free() override;

        TextureBuffer *getTextureBuffer();

    private:
        RenderAllocator *alloc;
        ImageResource *img;
        TextureBuffer::Attributes attrib;
        TextureBuffer *texture;
    };
}

#endif //MANA_TEXTUREBUFFERRESOURCE_HPP
