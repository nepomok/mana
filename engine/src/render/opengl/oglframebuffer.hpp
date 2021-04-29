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

#ifndef MANA_OGLFRAMEBUFFER_HPP
#define MANA_OGLFRAMEBUFFER_HPP

#include "engine/render/rendertarget.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLFrameBuffer : public RenderTarget {
        public:
            ~OGLFrameBuffer() override = default;

            Vec2i getSize() const override = 0;

            void blitColor(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           RenderTexture::TextureFiltering filter) override = 0;

            void blitDepth(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           RenderTexture::TextureFiltering filter) override = 0;

            void blitStencil(const RenderTarget &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             RenderTexture::TextureFiltering filter) override = 0;

            void attachColor(const RenderTexture &texture) override = 0;

            void attachDepth(const RenderTexture &texture) override = 0;

            void attachStencil(const RenderTexture &texture) override = 0;

            void attachDepthStencil(const RenderTexture &texture) override = 0;

            void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            virtual GLuint getFBO() const = 0;
        };
    }
}

#endif //MANA_OGLFRAMEBUFFER_HPP
