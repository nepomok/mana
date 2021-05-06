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

#ifndef MANA_OGLUSERFRAMEBUFFER_HPP
#define MANA_OGLUSERFRAMEBUFFER_HPP

#include "oglframebuffer.hpp"

typedef unsigned int GLuint;

namespace mana {
    namespace opengl {
        class OGLUserFrameBuffer : public OGLFrameBuffer {
        public:
            GLuint FBO;

            int width;
            int height;

            bool deleteFramebuffer = true;

            OGLUserFrameBuffer();

            OGLUserFrameBuffer(int width, int height);

            ~OGLUserFrameBuffer() override;

            Vec2i getSize() const override;

            void blitColor(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override;

            void blitDepth(const RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override;

            void blitStencil(const RenderTarget &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             TextureBuffer::TextureFiltering filter) override;

            void attachColor(const TextureBuffer &texture) override;

            void attachDepth(const TextureBuffer &texture) override;

            void attachStencil(const TextureBuffer &texture) override;

            void attachDepthStencil(const TextureBuffer &texture) override;

            void attachColor(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override;

            void attachDepth(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override;

            void attachStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override;

            void attachDepthStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override;

            GLuint getFBO() const override;
        };
    }
}

#endif //MANA_OGLUSERFRAMEBUFFER_HPP
