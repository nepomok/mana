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

#ifndef MANA_QTOGLRENDERTARGET_HPP
#define MANA_QTOGLRENDERTARGET_HPP

#include "engine/render/rendertarget.hpp"

#include <QOpenGLFunctions_3_3_Core>

typedef unsigned int GLuint;

namespace mana {
    namespace opengl {
        class QtOGLRenderTarget : public RenderTarget, public QOpenGLFunctions_3_3_Core {
        public:
            GLuint FBO;

            Vec2i size;

            bool deleteFramebuffer = true;

            QtOGLRenderTarget();

            QtOGLRenderTarget(Vec2i size, int samples);

            ~QtOGLRenderTarget() override;

            Vec2i getSize() override;

            void blitColor(RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override;

            void blitDepth(RenderTarget &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureBuffer::TextureFiltering filter) override;

            void blitStencil(RenderTarget &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             TextureBuffer::TextureFiltering filter) override;

            void attachColor(int index, TextureBuffer &texture) override;

            void attachDepth(TextureBuffer &texture) override;

            void attachStencil(TextureBuffer &texture) override;

            void attachDepthStencil(TextureBuffer &texture) override;

            void attachColor(int index, TextureBuffer::CubeMapFace face, TextureBuffer &texture) override;

            void attachDepth(TextureBuffer::CubeMapFace face, TextureBuffer &texture) override;

            void attachStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) override;

            void attachDepthStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) override;

            GLuint getFBO() const;
        };
    }
}

#endif //MANA_QTOGLRENDERTARGET_HPP
