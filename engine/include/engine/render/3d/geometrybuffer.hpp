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
    class GeometryBuffer {
    public:
        GeometryBuffer() = default;

        explicit GeometryBuffer(RenderAllocator &allocator, Vec2i size = {640, 320});

        ~GeometryBuffer();

        void setSize(const Vec2i &s);

        Vec2i getSize();

        /**
        * The geometry buffer textures are bound to the following targets:
        *
        * position     :   SV_TARGET0
        * normal       :   SV_TARGET1
        * diffuse      :   SV_TARGET2
        * ambient      :   SV_TARGET3
        * specular     :   SV_TARGET4
        * lighting     :   SV_TARGET5
        * id           :   SV_TARGET6
        *
        * Contained textures and render target are reallocated whenever the size changes.
        */
        RenderTarget &getRenderTarget();

        /**
         * 3 * 32 bit float
         * @return
         */
        TextureBuffer &getPosition();

        /**
         * 3 * 32 bit float
         * @return
         */
        TextureBuffer &getNormal();

        /**
         * 3 * 32 bit float
         * @return
         */
        TextureBuffer &getDiffuse();

        /**
         * 3 * 32 bit float
         * @return
         */
        TextureBuffer &getAmbient();

        /**
         * 3 * 32 bit float
         * @return
         */
        TextureBuffer &getSpecular();

        /**
         * 2 * 32 bit float
         *  [0] = shininess
         *  [1] = if this value is 0 the pixel color is diffuse color otherwise it is the phong shaded color
         * @return
         */
        TextureBuffer &getLighting();

        /**
         * 24 bit depth + 8 bit stencil
         * @return
         */
        TextureBuffer &getDepthStencil();

        /**
         * 1 * 32 bit unsigned integer
         *  [0] = id of the command that has drawn the pixel
         * @return
         */
        TextureBuffer &getId();

        MeshBuffer &getScreenQuad();

    private:
        RenderAllocator *renderAllocator{};
        RenderTarget *renderTarget{};

        Vec2i size;

        TextureBuffer *position{};
        TextureBuffer *normal{};
        TextureBuffer *diffuse{};
        TextureBuffer *ambient{};
        TextureBuffer *specular{};
        TextureBuffer *lighting{};
        TextureBuffer *depthStencil{};
        TextureBuffer *id{};

        MeshBuffer *screenQuad{};
    };
}

#endif //MANA_GEOMETRYBUFFER_HPP
