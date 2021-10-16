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

#include <memory>

#include "engine/render/renderdevice.hpp"
#include "engine/render/texturebuffer.hpp"

namespace engine {
    class GeometryBuffer {
    public:
        GeometryBuffer() = default;

        explicit GeometryBuffer(RenderAllocator &allocator, Vec2i size = {640, 320});

        ~GeometryBuffer();

        /**
         * Set the geometry buffer size, this reallocates the render target and buffers.
         *
         * @param s
         */
        void setSize(const Vec2i &s);

        Vec2i getSize();

        /**
         *
         * @return
         */
        RenderTarget &getRenderTarget();

        void addBuffer(const std::string &name, TextureBuffer::ColorFormat format);

        TextureBuffer &getBuffer(const std::string &name);

        /**
         * Attach the color buffers previously added to the render target.
         * The attachment point is SV_TARGET_INDEX_ where _INDEX_ is the position of the attachment name in the passed vector
         *
         * @param attachments
         */
        void attachColor(const std::vector<std::string> &attachments);

        void attachDepthStencil(const std::string &name);

        void detachDepthStencil();

        /**
         * A mesh buffer of a plane which fills viewport without a transformation.
         *
         * @return
         */
        MeshBuffer &getScreenQuad();

    private:
        RenderAllocator *renderAllocator{};
        RenderTarget *renderTarget{};

        Vec2i size;

        std::map<std::string, TextureBuffer::ColorFormat> formats;
        std::map<std::string, std::unique_ptr<TextureBuffer>> buffers;

        std::vector<std::string> currentColor;
        std::string currentDepthStencil;

        MeshBuffer *screenQuad{};
    };
}

#endif //MANA_GEOMETRYBUFFER_HPP
