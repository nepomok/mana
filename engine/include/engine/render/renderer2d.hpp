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

#ifndef MANA_RENDERER2D_HPP
#define MANA_RENDERER2D_HPP

#include "engine/render/renderer.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    /**
     * The 2d renderer uses the following coordinate system:
     *      -Y
     *       |
     * -X - -|- - +X
     *       |
     *      +Y
     * with 0,0 at the top left and depending on the argument types of the called function either:
     *  float(Normalized screen coordinates) = 1,1 the lower right
     *  int(Pixel coordinates) = viewport width - 1, viewport height - 1 at the lower right.
     */
    class Renderer2D {
    public:
        Renderer2D();

        Renderer2D(Renderer &ren, RenderAllocator &alloc);

        void setEnableAlphaBlending(bool enable);

        void renderBegin(const RenderTarget &target);

        void renderBegin(const RenderTarget &target,
                         Vec2i viewportOffset,
                         Vec2i viewportSize);

        void draw(Rectf srcRect, Rectf dstRect, const Texture& texture, const ShaderProgram& shader, Vec2f center, float rotation);

        void draw(Rectf srcRect, Rectf dstRect, const Texture& texture, const ShaderProgram& shader);

        void draw(Rectf srcRect, Rectf dstRect, const Texture& texture, Vec2f center, float rotation);

        void draw(Rectf srcRect, Rectf dstRect, const Texture& texture);

        void draw(Rectf rectangle, ColorRGBA color, bool fill, Vec2f center, float rotation);

        void draw(Rectf rectangle, ColorRGBA color = {}, bool fill = true);

        void draw(Vec2f start, Vec2f end, ColorRGBA color, Vec2f center, float rotation);

        void draw(Vec2f start, Vec2f end, ColorRGBA color = {});

        void draw(Vec2f point, ColorRGBA color = {});

        void draw(Recti srcRect, Recti dstRect, const Texture& texture, const ShaderProgram& shader, Vec2i center, float rotation);

        void draw(Recti srcRect, Recti dstRect, const Texture& texture, const ShaderProgram& shader);

        void draw(Recti srcRect, Recti dstRect, const Texture& texture, Vec2i center, float rotation);

        void draw(Recti srcRect, Recti dstRect, const Texture& texture);

        void draw(Recti rectangle, ColorRGBA color, bool fill, Vec2i center, float rotation);

        void draw(Recti rectangle, ColorRGBA color = {}, bool fill = true);

        void draw(Vec2i start, Vec2i end, ColorRGBA color, Vec2i center, float rotation);

        void draw(Vec2i start, Vec2i end, ColorRGBA color = {});

        void draw(Vec2i point, ColorRGBA color = {});

        void renderPresent();

    private:
        Renderer *ren = nullptr;
        RenderAllocator *alloc = nullptr;
    };
}

#endif //MANA_RENDERER2D_HPP
