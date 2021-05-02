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

#include "ogluserframebuffer.hpp"
#include "ogltypeconverter.hpp"
#include "oglcheckerror.hpp"
#include "oglrendertexture.hpp"

using namespace mana;
using namespace mana::opengl;

opengl::OGLUserFrameBuffer::OGLUserFrameBuffer() : FBO(0),
                                                   width(0),
                                                   height(0)
                                                   {}

opengl::OGLUserFrameBuffer::OGLUserFrameBuffer(int width, int height) : width(width),
                                                                        height(height),
                                                                        FBO(0)
                                                                        {}

opengl::OGLUserFrameBuffer::~OGLUserFrameBuffer() {
    glDeleteFramebuffers(1, &FBO);
}

Vec2i opengl::OGLUserFrameBuffer::getSize() const {
    return {width, height};
}

void opengl::OGLUserFrameBuffer::blitColor(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                           Vec2i sourceRect, Vec2i targetRect, RenderTexture::TextureFiltering filter) {
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
                      OGLTypeConverter::convert(filter));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkGLError("OGLUserFrameBuffer::blitFramebuffer");
}

void opengl::OGLUserFrameBuffer::blitDepth(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                           Vec2i sourceRect, Vec2i targetRect, RenderTexture::TextureFiltering filter) {
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
                      OGLTypeConverter::convert(filter));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkGLError("OGLUserFrameBuffer::blitFramebuffer");
}

void opengl::OGLUserFrameBuffer::blitStencil(const RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                             Vec2i sourceRect, Vec2i targetRect, RenderTexture::TextureFiltering filter) {
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
                      OGLTypeConverter::convert(filter));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkGLError("OGLUserFrameBuffer::blitFramebuffer");
}

void opengl::OGLUserFrameBuffer::attachColor(const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachDepth(const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachStencil(const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachDepthStencil(const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, OGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

void opengl::OGLUserFrameBuffer::attachDepthStencil(RenderTexture::CubeMapFace face,
                                                    const RenderTexture &texture) {
    auto &tex = dynamic_cast<const OGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLError("");
}

GLuint opengl::OGLUserFrameBuffer::getFBO() const {
    return FBO;
}
