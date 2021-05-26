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

#ifndef MANA_RENDERTARGET_HPP
#define MANA_RENDERTARGET_HPP

#include "engine/render/renderobject.hpp"
#include "engine/render/texturebuffer.hpp"

namespace mana {
    class RenderTarget : public RenderObject {
    public:
        ~RenderTarget() override = default;

        virtual Vec2i getSize() = 0;

        virtual void blitColor(RenderTarget &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureBuffer::TextureFiltering filter) = 0;

        virtual void blitDepth(RenderTarget &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureBuffer::TextureFiltering filter) = 0;

        virtual void blitStencil(RenderTarget &source,
                                 Vec2i sourceOffset,
                                 Vec2i targetOffset,
                                 Vec2i sourceRect,
                                 Vec2i targetRect,
                                 TextureBuffer::TextureFiltering filter) = 0;

        virtual void attachColor(TextureBuffer &texture) = 0;

        virtual void attachDepth(TextureBuffer &texture) = 0;

        virtual void attachStencil(TextureBuffer &texture) = 0;

        virtual void attachDepthStencil(TextureBuffer &texture) = 0;

        virtual void attachColor(TextureBuffer::CubeMapFace face, TextureBuffer &texture) = 0;

        virtual void attachDepth(TextureBuffer::CubeMapFace face, TextureBuffer &texture) = 0;

        virtual void attachStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) = 0;

        virtual void attachDepthStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) = 0;
    };
}

#endif //MANA_RENDERTARGET_HPP
