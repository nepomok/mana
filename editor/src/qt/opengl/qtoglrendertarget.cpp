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

#include "qtoglrendertarget.hpp"
#include "qtogltypeconverter.hpp"
#include "qtoglcheckerror.hpp"
#include "qtoglrendertexture.hpp"
#include "qtoglcheckerror.hpp"

using namespace mana;
using namespace mana::opengl;

opengl::QtOGLRenderTarget::QtOGLRenderTarget() : FBO(0),
                                                 size() {}

opengl::QtOGLRenderTarget::QtOGLRenderTarget(Vec2i size, int samples) : size(size),
                                                                        FBO(0) {
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenRenderbuffers(1, &colorRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRBO);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, size.x, size.y);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRBO);

    glGenRenderbuffers(1, &depthStencilRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRBO);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, size.x, size.y);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Failed to setup framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkQtGLError("OGLRenderAllocator::allocateFrameBuffer");
}

opengl::QtOGLRenderTarget::~QtOGLRenderTarget() {
    if (deleteFramebuffer)
        glDeleteFramebuffers(1, &FBO);
}

Vec2i opengl::QtOGLRenderTarget::getSize() {
    return size;
}

void opengl::QtOGLRenderTarget::blitColor(RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                          Vec2i sourceRect, Vec2i targetRect, TextureBuffer::TextureFiltering filter) {
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

    auto &fbS = dynamic_cast<QtOGLRenderTarget &>(source);

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
                      QtOGLTypeConverter::convert(filter));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkQtGLError("QtOGLUserFrameBuffer::blitFramebuffer");
}

void opengl::QtOGLRenderTarget::blitDepth(RenderTarget &source,
                                          Vec2i sourceOffset,
                                          Vec2i targetOffset,
                                          Vec2i sourceRect,
                                          Vec2i targetRect) {
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

    auto &fbS = dynamic_cast<QtOGLRenderTarget &>(source);

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
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkQtGLError("QtOGLUserFrameBuffer::blitFramebuffer");
}

void opengl::QtOGLRenderTarget::blitStencil(RenderTarget &source,
                                            Vec2i sourceOffset,
                                            Vec2i targetOffset,
                                            Vec2i sourceRect,
                                            Vec2i targetRect) {
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

    auto &fbS = dynamic_cast<QtOGLRenderTarget &>(source);

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
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkQtGLError("QtOGLUserFrameBuffer::blitFramebuffer");
}

void QtOGLRenderTarget::setNumberOfColorAttachments(int count) {
    unsigned int attachments[count];
    for (int i = 0; i < count; i++) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glDrawBuffers(count, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachColor(int index, TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachDepth(TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachStencil(TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachDepthStencil(TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachColor(int index, TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, QtOGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachDepth(TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, QtOGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, QtOGLTypeConverter::convert(face), tex.handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

void opengl::QtOGLRenderTarget::attachDepthStencil(TextureBuffer::CubeMapFace face,
                                                   TextureBuffer &texture) {
    auto &tex = dynamic_cast<QtOGLRenderTexture &>(texture);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, QtOGLTypeConverter::convert(face), tex.handle,
                           0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkQtGLError("");
}

GLuint opengl::QtOGLRenderTarget::getFBO() const {
    return FBO;
}

int QtOGLRenderTarget::getSamples() {
    return 0;
}

void QtOGLRenderTarget::detachColor(int index) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void QtOGLRenderTarget::detachDepth() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void QtOGLRenderTarget::detachStencil() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void QtOGLRenderTarget::detachDepthStencil() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
