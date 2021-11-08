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

#include "engine/render/3d/geometrybuffer.hpp"

namespace engine {
    GeometryBuffer::GeometryBuffer(RenderAllocator &allocator, Vec2i size) : renderAllocator(allocator), size(size) {
        renderTarget = allocator.createRenderTarget(size, 0);

        const Mesh quadMesh(Mesh::TRI,
                            {
                                    Vertex({-1, 1, 0}, {0, 1}),
                                    Vertex({1, 1, 0}, {1, 1}),
                                    Vertex({1, -1, 0}, {1, 0}),
                                    Vertex({-1, 1, 0}, {0, 1}),
                                    Vertex({1, -1, 0}, {1, 0}),
                                    Vertex({-1, -1, 0}, {0, 0})
                            });

        screenQuad = allocator.createMeshBuffer(quadMesh);
    }

    GeometryBuffer::~GeometryBuffer() {
        renderTarget->detachDepthStencil();
    }

    void GeometryBuffer::setSize(const Vec2i &s) {
        if (size == s)
            return;

        size = s;

        for (int i = 0; i < currentColor.size(); i++)
            renderTarget->detachColor(i);

        renderTarget->detachDepthStencil();

        for (auto &buf : buffers) {
            TextureBuffer::Attributes attr;
            attr.size = size;
            attr.format = formats.at(buf.first);
            buf.second = renderAllocator.createTextureBuffer(attr);
        }

        renderTarget = renderAllocator.createRenderTarget(size, 0);

        if (!currentDepthStencil.empty())
            renderTarget->attachDepthStencil(*buffers.at(currentDepthStencil));

        renderTarget->setNumberOfColorAttachments(static_cast<int>(currentColor.size()));
        for (int i = 0; i < currentColor.size(); i++) {
            renderTarget->attachColor(i, *buffers.at(currentColor.at(i)));
        }
    }

    Vec2i GeometryBuffer::getSize() {
        return size;
    }

    RenderTarget &GeometryBuffer::getRenderTarget() {
        assert(renderTarget != nullptr);
        return *renderTarget;
    }

    void GeometryBuffer::addBuffer(const std::string &name, TextureBuffer::ColorFormat format) {
        auto it = buffers.find(name);
        if (it != buffers.end()) {
            if (it->second->getAttributes().format != format)
                throw std::runtime_error("Buffer with different format already exists " + name);
            return;
        }

        TextureBuffer::Attributes attr;
        attr.size = size;
        attr.format = format;
        buffers[name] = renderAllocator.createTextureBuffer(attr);
        formats[name] = format;
    }

    TextureBuffer &GeometryBuffer::getBuffer(const std::string &name) {
        return *buffers.at(name);
    }

    void GeometryBuffer::attachColor(const std::vector<std::string> &attachments) {
        for (int i = 0; i < currentColor.size(); i++)
            renderTarget->detachColor(i);
        renderTarget->setNumberOfColorAttachments(static_cast<int>(attachments.size()));
        for (int i = 0; i < attachments.size(); i++)
            renderTarget->attachColor(i, *buffers.at(attachments.at(i)));
        currentColor = attachments;
    }

    void GeometryBuffer::attachDepthStencil(const std::string &name) {
        auto &buffer = getBuffer(name);
        if (buffer.getAttributes().format != TextureBuffer::ColorFormat::DEPTH_STENCIL)
            throw std::runtime_error("Invalid depth stencil texture " + name);
        renderTarget->detachDepthStencil();
        renderTarget->attachDepthStencil(buffer);
        currentDepthStencil = name;
    }

    void GeometryBuffer::detachDepthStencil() {
        renderTarget->detachDepthStencil();
        currentDepthStencil = "";
    }

    MeshBuffer &GeometryBuffer::getScreenQuad() {
        assert(screenQuad != nullptr);
        return *screenQuad;
    }
}