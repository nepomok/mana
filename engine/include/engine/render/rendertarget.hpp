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
#include "engine/render/texture.hpp"

namespace mana {
    class RenderTarget : public RenderObject {
    public:
        ~RenderTarget() override = default;

        virtual Vec2i getSize() const = 0;

        virtual void blitColor(const RenderTarget &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               Texture::TextureFiltering filter) = 0;

        virtual void blitDepth(const RenderTarget &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               Texture::TextureFiltering filter) = 0;

        virtual void blitStencil(const RenderTarget &source,
                                 Vec2i sourceOffset,
                                 Vec2i targetOffset,
                                 Vec2i sourceRect,
                                 Vec2i targetRect,
                                 Texture::TextureFiltering filter) = 0;

        virtual void attachColor(const Texture &texture) = 0;

        virtual void attachDepth(const Texture &texture) = 0;

        virtual void attachStencil(const Texture &texture) = 0;

        virtual void attachDepthStencil(const Texture &texture) = 0;

        virtual void attachColor(Texture::CubeMapFace face, const Texture &texture) = 0;

        virtual void attachDepth(Texture::CubeMapFace face, const Texture &texture) = 0;

        virtual void attachStencil(Texture::CubeMapFace face, const Texture &texture) = 0;

        virtual void attachDepthStencil(Texture::CubeMapFace face, const Texture &texture) = 0;
    };
}

#endif //MANA_RENDERTARGET_HPP
