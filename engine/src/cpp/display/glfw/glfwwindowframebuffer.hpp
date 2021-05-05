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

#ifndef MANA_GLFWWINDOWFRAMEBUFFER_HPP
#define MANA_GLFWWINDOWFRAMEBUFFER_HPP

#include "engine/render/opengl/oglframebuffer.hpp"

#include "render/opengl/ogltypeconverter.hpp"
#include "render/opengl/oglcheckerror.hpp"

namespace mana {
    class GLFWWindowFrameBuffer : public opengl::OGLFrameBuffer {
    public:
        GLFWwindow *wndH;

        explicit GLFWWindowFrameBuffer(GLFWwindow *wndH) : wndH(wndH) {}

        Vec2i getSize() const override {
            Vec2i ret;
            glfwGetWindowSize(wndH, &ret.x, &ret.y);
            return ret;
        };

        void blitColor(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                       Vec2i sourceRect, Vec2i targetRect, TextureBuffer::TextureFiltering filter) override{
            if (sourceRect.x < 0 || sourceRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (sourceOffset.x < 0 || sourceOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }
            if (targetRect.x < 0 || targetRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (targetOffset.x < 0 || targetOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }

            auto &fbS = dynamic_cast<const OGLFrameBuffer &>(source);

            Vec2i sourceSize = fbS.getSize();
            if (sourceSize.x < sourceRect.x + sourceOffset.x || sourceSize.y < sourceRect.y + sourceOffset.y)
                throw std::runtime_error("Blit rect out of bounds for source framebuffer");

            Vec2i targetSize = getSize();
            if (targetSize.x < targetRect.x + targetOffset.x || targetSize.y < targetRect.y + targetOffset.y)
                throw std::runtime_error("Blit rect out of bounds for target framebuffer.");

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbS.getFBO());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO());
            glBlitFramebuffer(sourceOffset.x,
                              sourceOffset.y,
                              sourceRect.x,
                              sourceRect.y,
                              targetOffset.x,
                              targetOffset.y,
                              targetRect.x,
                              targetRect.y,
                              GL_COLOR_BUFFER_BIT,
                              opengl::OGLTypeConverter::convert(filter));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("GLFWWindowFrameBuffer::blitFramebuffer");
        }

        void blitDepth(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                       Vec2i sourceRect, Vec2i targetRect, TextureBuffer::TextureFiltering filter) override {
            if (sourceRect.x < 0 || sourceRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (sourceOffset.x < 0 || sourceOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }
            if (targetRect.x < 0 || targetRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (targetOffset.x < 0 || targetOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }

            auto &fbS = dynamic_cast<const OGLFrameBuffer &>(source);

            Vec2i sourceSize = fbS.getSize();
            if (sourceSize.x < sourceRect.x + sourceOffset.x || sourceSize.y < sourceRect.y + sourceOffset.y)
                throw std::runtime_error("Blit rect out of bounds for source framebuffer");

            Vec2i targetSize = getSize();
            if (targetSize.x < targetRect.x + targetOffset.x || targetSize.y < targetRect.y + targetOffset.y)
                throw std::runtime_error("Blit rect out of bounds for target framebuffer.");

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbS.getFBO());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO());
            glBlitFramebuffer(sourceOffset.x,
                              sourceOffset.y,
                              sourceRect.x,
                              sourceRect.y,
                              targetOffset.x,
                              targetOffset.y,
                              targetRect.x,
                              targetRect.y,
                              GL_DEPTH_BUFFER_BIT,
                              opengl::OGLTypeConverter::convert(filter));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("GLFWWindowFrameBuffer::blitFramebuffer");
        }

        void blitStencil(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                         Vec2i sourceRect, Vec2i targetRect, TextureBuffer::TextureFiltering filter) override{
            if (sourceRect.x < 0 || sourceRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (sourceOffset.x < 0 || sourceOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }
            if (targetRect.x < 0 || targetRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (targetOffset.x < 0 || targetOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }

            auto &fbS = dynamic_cast<const OGLFrameBuffer &>(source);

            Vec2i sourceSize = fbS.getSize();
            if (sourceSize.x < sourceRect.x + sourceOffset.x || sourceSize.y < sourceRect.y + sourceOffset.y)
                throw std::runtime_error("Blit rect out of bounds for source framebuffer");

            Vec2i targetSize = getSize();
            if (targetSize.x < targetRect.x + targetOffset.x || targetSize.y < targetRect.y + targetOffset.y)
                throw std::runtime_error("Blit rect out of bounds for target framebuffer.");

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbS.getFBO());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO());
            glBlitFramebuffer(sourceOffset.x,
                              sourceOffset.y,
                              sourceRect.x,
                              sourceRect.y,
                              targetOffset.x,
                              targetOffset.y,
                              targetRect.x,
                              targetRect.y,
                              GL_STENCIL_BUFFER_BIT,
                              opengl::OGLTypeConverter::convert(filter));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("OGLUserFrameBuffer::blitFramebuffer");
        }

        void attachColor(const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepth(const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachStencil(const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepthStencil(const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachColor(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepth(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepthStencil(TextureBuffer::CubeMapFace face, const TextureBuffer &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        GLuint getFBO() const override {
            return 0;
        };
    };
}

#endif //MANA_GLFWWINDOWFRAMEBUFFER_HPP
