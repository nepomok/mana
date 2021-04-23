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

#ifndef MANA_GLFWWINDOWFRAMEBUFFER_HPP
#define MANA_GLFWWINDOWFRAMEBUFFER_HPP

#include "render/opengl/oglframebuffer.hpp"

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

        void blitColor(const FrameBuffer &source,
                       Vec2i sourceOffset,
                       Vec2i targetOffset,
                       Vec2i sourceRect,
                       Vec2i targetRect,
                       TextureFiltering filter) override {
            throw std::runtime_error("Not Implemented");
        };

        void blitDepth(const FrameBuffer &source,
                       Vec2i sourceOffset,
                       Vec2i targetOffset,
                       Vec2i sourceRect,
                       Vec2i targetRect,
                       TextureFiltering filter) override {
            throw std::runtime_error("Not Implemented");
        };

        void blitStencil(const FrameBuffer &source,
                         Vec2i sourceOffset,
                         Vec2i targetOffset,
                         Vec2i sourceRect,
                         Vec2i targetRect,
                         TextureFiltering filter) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachColor(const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepth(const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachStencil(const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepthStencil(const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override {
            throw std::runtime_error("Not Implemented");
        };

        GLuint getFBO() const override {
            return 0;
        };
    };
}

#endif //MANA_GLFWWINDOWFRAMEBUFFER_HPP
