/**
 *  Mana - 3D Engine
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

#ifndef MANA_FRAMEBUFFER_HPP
#define MANA_FRAMEBUFFER_HPP

#include "engine/render/renderobject.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/textureattributes.hpp"

namespace mana {
    class FrameBuffer : public RenderObject {
    public:
        ~FrameBuffer() override = default;

        virtual Vec2i getSize() const = 0;

        virtual void blitColor(const FrameBuffer &source,
                          Vec2i sourceOffset,
                          Vec2i targetOffset,
                          Vec2i sourceRect,
                          Vec2i targetRect,
                          TextureFiltering filter) = 0;

        virtual void blitDepth(const FrameBuffer &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureFiltering filter) = 0;

        virtual void blitStencil(const FrameBuffer &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureFiltering filter) = 0;

        virtual void attachColor(const RenderTexture &texture) = 0;

        virtual void attachDepth(const RenderTexture &texture) = 0;

        virtual void attachStencil(const RenderTexture &texture) = 0;

        virtual void attachDepthStencil(const RenderTexture &texture) = 0;

        virtual void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;
    };
}

#endif //MANA_FRAMEBUFFER_HPP
