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

#include "render/opengl/oglrendertarget.hpp"
#include "render/opengl/ogltypeconverter.hpp"
#include "render/opengl/oglcheckerror.hpp"
#include "render/opengl/ogltexturebuffer.hpp"

namespace mana {
    opengl::OGLRenderTarget::OGLRenderTarget() : FBO(), colorRBO(), depthStencilRBO(), size(), samples() {}

    opengl::OGLRenderTarget::OGLRenderTarget(Vec2i size, int samples)
            : FBO(), colorRBO(), depthStencilRBO(), size(size), samples(samples) {
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

        checkGLError("OGLRenderAllocator::allocateFrameBuffer");
    }

    opengl::OGLRenderTarget::~OGLRenderTarget() {
        glDeleteFramebuffers(1, &FBO);
    }

    Vec2i opengl::OGLRenderTarget::getSize() {
        return size;
    }

    int opengl::OGLRenderTarget::getSamples() {
        return samples;
    }

    void opengl::OGLRenderTarget::blitColor(RenderTarget &source,
                                            Vec2i sourceOffset,
                                            Vec2i targetOffset,
                                            Vec2i sourceRect,
                                            Vec2i targetRect,
                                            TextureBuffer::TextureFiltering filter) {
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

        auto &fbS = dynamic_cast<OGLRenderTarget &>(source);

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

    void opengl::OGLRenderTarget::blitDepth(RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                            Vec2i sourceRect, Vec2i targetRect,
                                            TextureBuffer::TextureFiltering filter) {
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

        auto &fbS = dynamic_cast< OGLRenderTarget &>(source);

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

        checkGLError("OGLUserFrameBuffer::blitFramebuffer");
    }

    void opengl::OGLRenderTarget::blitStencil(RenderTarget &source, Vec2i sourceOffset, Vec2i targetOffset,
                                              Vec2i sourceRect, Vec2i targetRect,
                                              TextureBuffer::TextureFiltering filter) {
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

        auto &fbS = dynamic_cast< OGLRenderTarget &>(source);

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

    void opengl::OGLRenderTarget::setNumberOfColorAttachments(int count) {
        unsigned int attachments[count];
        for (int i = 0; i < count; i++) {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glDrawBuffers(count, attachments);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError();
    }

    void opengl::OGLRenderTarget::attachColor(int index, TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachDepth(TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachStencil(TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachDepthStencil(TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachColor(int index, TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, OGLTypeConverter::convert(face), tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachDepth(TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachStencil(TextureBuffer::CubeMapFace face, TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    void opengl::OGLRenderTarget::attachDepthStencil(TextureBuffer::CubeMapFace face,
                                                     TextureBuffer &texture) {
        auto &tex = dynamic_cast< OGLTextureBuffer &>(texture);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, OGLTypeConverter::convert(face), tex.handle,
                               0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        checkGLError("");
    }

    GLuint opengl::OGLRenderTarget::getFBO() {
        return FBO;
    }

    void opengl::OGLRenderTarget::detachColor(int index) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, colorRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void opengl::OGLRenderTarget::detachDepth() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void opengl::OGLRenderTarget::detachStencil() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void opengl::OGLRenderTarget::detachDepthStencil() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}