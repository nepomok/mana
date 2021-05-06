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

#ifndef MANA_QTOGLFRAMEBUFFER_HPP
#define MANA_QTOGLFRAMEBUFFER_HPP

#include "engine/render/rendertarget.hpp"

typedef unsigned int GLuint;

namespace mana {
    namespace opengl {
        class QtOGLFrameBuffer : public RenderTarget {
        public:
            ~QtOGLFrameBuffer() override = default;

            Vec2i getSize() const override = 0;

            void blitColor(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override = 0;

            void blitDepth(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override = 0;

            void blitStencil(const RenderTarget &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             TextureBuffer::TextureFiltering filter) override = 0;

            void attachColor(const TextureBuffer &texture) override = 0;

            void attachDepth(const TextureBuffer &texture) override = 0;

            void attachStencil(const TextureBuffer &texture) override = 0;

            void attachDepthStencil(const TextureBuffer &texture) override = 0;

            void attachColor(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override = 0;

            void attachDepth(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override = 0;

            void attachStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override = 0;

            void attachDepthStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override = 0;

            virtual GLuint getFBO() const = 0;
        };
    }
}

#endif //MANA_QTOGLFRAMEBUFFER_HPP
