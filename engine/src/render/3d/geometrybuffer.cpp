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

namespace mana {
    GeometryBuffer::GeometryBuffer(RenderAllocator &allocator, Vec2i size) : renderAllocator(&allocator), size(size) {
        TextureBuffer::Attributes attr;
        attr.size = size;
        attr.format = TextureBuffer::RGB32F;

        position = allocator.createTextureBuffer(attr);
        normal = allocator.createTextureBuffer(attr);
        attr.format = TextureBuffer::RGB;
        diffuse = allocator.createTextureBuffer(attr);
        ambient = allocator.createTextureBuffer(attr);
        specular = allocator.createTextureBuffer(attr);

        attr.format = TextureBuffer::RG32F;
        lighting = allocator.createTextureBuffer(attr);

        attr.format = TextureBuffer::R32UI;
        id = allocator.createTextureBuffer(attr);

        renderTarget = allocator.createRenderTarget(size, 0);
        renderTarget->setNumberOfColorAttachments(6);
        renderTarget->attachColor(0, *position);
        renderTarget->attachColor(1, *normal);
        renderTarget->attachColor(2, *diffuse);
        renderTarget->attachColor(3, *ambient);
        renderTarget->attachColor(4, *specular);
        renderTarget->attachColor(5, *lighting);
        renderTarget->attachColor(6, *id);

        attr.format = TextureBuffer::DEPTH_STENCIL;
        depthStencil = allocator.createTextureBuffer(attr);

        renderTarget->attachDepthStencil(*depthStencil);

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
        for (int i = 0; i < 6; i++)
            renderTarget->detachColor(i);
        renderTarget->detachDepthStencil();
        delete position;
        delete normal;
        delete diffuse;
        delete ambient;
        delete specular;
        delete lighting;
        delete depthStencil;
        delete id;
        delete renderTarget;
        delete screenQuad;
    }

    void GeometryBuffer::setSize(const Vec2i &s) {
        if (size == s)
            return;

        size = s;

        for (int i = 0; i < 6; i++)
            renderTarget->detachColor(i);

        renderTarget->detachDepthStencil();

        delete position;
        delete normal;
        delete diffuse;
        delete ambient;
        delete specular;
        delete lighting;
        delete depthStencil;
        delete id;
        delete renderTarget;

        auto &allocator = *renderAllocator;

        TextureBuffer::Attributes attr;
        attr.size = size;
        attr.format = TextureBuffer::RGB32F;
        position = allocator.createTextureBuffer(attr);
        normal = allocator.createTextureBuffer(attr);
        attr.format = TextureBuffer::RGB;
        diffuse = allocator.createTextureBuffer(attr);
        ambient = allocator.createTextureBuffer(attr);
        specular = allocator.createTextureBuffer(attr);
        attr.format = TextureBuffer::RG32F;
        lighting = allocator.createTextureBuffer(attr);

        attr.format = TextureBuffer::R32UI;
        id = allocator.createTextureBuffer(attr);

        renderTarget = allocator.createRenderTarget(size, 0);
        renderTarget->setNumberOfColorAttachments(6);
        renderTarget->attachColor(0, *position);
        renderTarget->attachColor(1, *normal);
        renderTarget->attachColor(2, *diffuse);
        renderTarget->attachColor(3, *ambient);
        renderTarget->attachColor(4, *specular);
        renderTarget->attachColor(5, *lighting);
        renderTarget->attachColor(6, *id);

        attr.format = TextureBuffer::DEPTH_STENCIL;
        depthStencil = allocator.createTextureBuffer(attr);

        renderTarget->attachDepthStencil(*depthStencil);
    }

    Vec2i GeometryBuffer::getSize() {
        return size;
    }

    RenderTarget &GeometryBuffer::getRenderTarget() {
        assert(renderTarget != nullptr);
        return *renderTarget;
    }

    TextureBuffer &GeometryBuffer::getPosition() {
        assert(position != nullptr);
        return *position;
    }

    TextureBuffer &GeometryBuffer::getNormal() {
        assert(normal != nullptr);
        return *normal;
    }

    TextureBuffer &GeometryBuffer::getDiffuse() {
        assert(diffuse != nullptr);
        return *diffuse;
    }

    TextureBuffer &GeometryBuffer::getAmbient() {
        assert(ambient != nullptr);
        return *ambient;
    }

    TextureBuffer &GeometryBuffer::getSpecular() {
        assert(specular != nullptr);
        return *specular;
    }

    TextureBuffer &GeometryBuffer::getLighting() {
        assert(lighting != nullptr);
        return *lighting;
    }

    TextureBuffer &GeometryBuffer::getDepthStencil() {
        assert(depthStencil != nullptr);
        return *depthStencil;
    }

    TextureBuffer &GeometryBuffer::getId() {
        assert(id != nullptr);
        return *id;
    }

    MeshBuffer &GeometryBuffer::getScreenQuad() {
        assert(screenQuad != nullptr);
        return *screenQuad;
    }
}