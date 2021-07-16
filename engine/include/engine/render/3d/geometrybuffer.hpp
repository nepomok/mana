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

#ifndef MANA_GEOMETRYBUFFER_HPP
#define MANA_GEOMETRYBUFFER_HPP

#include "engine/render/renderdevice.hpp"

#include "engine/render/texturebuffer.hpp"

namespace mana {
    /**
     * The geometry buffer textures are bound to the following targets:
     *
     * position     :   SV_TARGET0
     * normal       :   SV_TARGET1
     * diffuse      :   SV_TARGET2
     * ambient      :   SV_TARGET3
     * specular     :   SV_TARGET4
     * shininess    :   SV_TARGET5
     *
     * Contained textures and render target are reallocated whenever the size changes.
     */
    class GeometryBuffer {
    public:
        GeometryBuffer() = default;

        explicit GeometryBuffer(RenderDevice &device, Vec2i size = {640, 320});

        ~GeometryBuffer();

        void setSize(const Vec2i &s);

        Vec2i getSize();

        RenderDevice &getRenderDevice();

        RenderTarget &getRenderTarget();

        TextureBuffer &getPosition();

        TextureBuffer &getNormal();

        TextureBuffer &getDiffuse();

        TextureBuffer &getAmbient();

        TextureBuffer &getSpecular();

        TextureBuffer &getShininess();

        TextureBuffer &getDepthStencil();

    private:
        RenderDevice *renderDevice{};
        Vec2i size;

        RenderTarget *renderTarget{};

        TextureBuffer *position{};
        TextureBuffer *normal{};
        TextureBuffer *diffuse{};
        TextureBuffer *ambient{};
        TextureBuffer *specular{};
        TextureBuffer *shininess{};

        TextureBuffer *depthStencil{};
    };
}

#endif //MANA_GEOMETRYBUFFER_HPP
