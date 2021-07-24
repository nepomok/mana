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
    GeometryBuffer::GeometryBuffer(RenderDevice &device, Vec2i size) : renderDevice(&device), size(size) {
        TextureBuffer::Attributes attr;
        attr.size = size;
        attr.format = TextureBuffer::RGB32F;
        position = device.createTextureBuffer(attr);
        normal = device.createTextureBuffer(attr);
        attr.format = TextureBuffer::RGB;
        diffuse = device.createTextureBuffer(attr);
        ambient = device.createTextureBuffer(attr);
        specular = device.createTextureBuffer(attr);
        attr.format = TextureBuffer::R32F;
        shininess = device.createTextureBuffer(attr);

        renderTarget = device.createRenderTarget(size, 0);
        renderTarget->setNumberOfColorAttachments(6);
        renderTarget->attachColor(0, *position);
        renderTarget->attachColor(1, *normal);
        renderTarget->attachColor(2, *diffuse);
        renderTarget->attachColor(3, *ambient);
        renderTarget->attachColor(4, *specular);
        renderTarget->attachColor(5, *shininess);

        attr.format = TextureBuffer::DEPTH_STENCIL;
        depthStencil = renderDevice->createTextureBuffer(attr);

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
        screenQuad = device.createMeshBuffer(quadMesh);
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
        delete shininess;
        delete depthStencil;
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
        delete shininess;
        delete depthStencil;
        delete renderTarget;

        TextureBuffer::Attributes attr;
        attr.size = size;
        attr.format = TextureBuffer::RGB32F;
        position = renderDevice->createTextureBuffer(attr);
        normal = renderDevice->createTextureBuffer(attr);
        attr.format = TextureBuffer::RGB;
        diffuse = renderDevice->createTextureBuffer(attr);
        ambient = renderDevice->createTextureBuffer(attr);
        specular = renderDevice->createTextureBuffer(attr);
        attr.format = TextureBuffer::R32F;
        shininess = renderDevice->createTextureBuffer(attr);

        renderTarget = renderDevice->createRenderTarget(size, 0);
        renderTarget->setNumberOfColorAttachments(6);
        renderTarget->attachColor(0, *position);
        renderTarget->attachColor(1, *normal);
        renderTarget->attachColor(2, *diffuse);
        renderTarget->attachColor(3, *ambient);
        renderTarget->attachColor(4, *specular);
        renderTarget->attachColor(5, *shininess);

        attr.format = TextureBuffer::DEPTH_STENCIL;
        depthStencil = renderDevice->createTextureBuffer(attr);

        renderTarget->attachDepthStencil(*depthStencil);
    }

    Vec2i GeometryBuffer::getSize() {
        return size;
    }

    RenderDevice &GeometryBuffer::getRenderDevice() {
        assert(renderDevice != nullptr);
        return *renderDevice;
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

    TextureBuffer &GeometryBuffer::getShininess() {
        assert(shininess != nullptr);
        return *shininess;
    }

    TextureBuffer &GeometryBuffer::getDepthStencil() {
        assert(depthStencil != nullptr);
        return *depthStencil;
    }

    MeshBuffer &GeometryBuffer::getScreenQuad() {
        assert(screenQuad != nullptr);
        return *screenQuad;
    }
}