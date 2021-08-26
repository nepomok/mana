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
#include "engine/render/renderdevice.hpp"
#include "engine/render/2d/text/character.hpp"

namespace mana {
    /**
     * This is a SDL inspired 2d renderer.
     * It uses a similar interface to SDL in a object oriented manner.
     *
     * The 2d renderer uses the following coordinate system:
     *      -Y
     *       |
     * -X - -|- - +X
     *       |
     *      +Y
     *
     * with 0,0 at the top left and depending on the argument types of the called function either:
     *
     *  float(Normalized screen coordinates) = 1,1 the lower right
     *  int(Pixel coordinates) = viewport width - 1, viewport height - 1 at the lower right.
     *
     * The Renderer2D ignores depth information stored in the render target.
     * The order in which methods are invoked controls which elements are drawn below / above.
     *
     * For example when drawing a rectangle texture first and then drawing a triangle texture afterwards
     * the triangle will be drawn on top of the rectangle.
     */
    class Renderer2D {
    public:
        Renderer2D();

        explicit Renderer2D(RenderDevice &device);

        void renderBegin(const RenderTarget &target, bool clear);

        void renderBegin(const RenderTarget &target,
                         bool clear,
                         Vec2i viewportOffset,
                         Vec2i viewportSize);

        void draw(Rectf srcRect,
                  Rectf dstRect,
                  const TextureBuffer &texture,
                  const ShaderProgram &shader,
                  Vec2f center,
                  float rotation);

        void draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture, const ShaderProgram &shader);

        void draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture, Vec2f center, float rotation);

        void draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture);

        void draw(Rectf rectangle, ColorRGBA color, bool fill, Vec2f center, float rotation);

        void draw(Rectf rectangle, ColorRGBA color = {}, bool fill = true);

        void draw(Vec2f start, Vec2f end, ColorRGBA color, Vec2f center, float rotation);

        void draw(Vec2f start, Vec2f end, ColorRGBA color = {});

        void draw(Vec2f point, ColorRGBA color = {});

        void draw(Recti srcRect,
                  Recti dstRect,
                  const TextureBuffer &texture,
                  const ShaderProgram &shader,
                  Vec2i center,
                  float rotation);

        void draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture, const ShaderProgram &shader);

        void draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture, Vec2i center, float rotation);

        void draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture);

        void draw(Recti rectangle, ColorRGBA color, bool fill, Vec2i center, float rotation);

        void draw(Recti rectangle, ColorRGBA color = {}, bool fill = true);

        void draw(Vec2i start, Vec2i end, ColorRGBA color, Vec2i center, float rotation);

        void draw(Vec2i start, Vec2i end, ColorRGBA color = {});

        void draw(Vec2i point, ColorRGBA color = {});

        /**
         * Convenience method which handles the spacing of characters in the mapping by rendering
         * a user defined ascii string.
         */
        void draw(Vec2i pos, const std::string &text, std::map<char, Character> &mapping, ColorRGBA color);

        void draw(Vec2f pos, const std::string &text, std::map<char, Character> &mapping, ColorRGBA color);

        void draw(Vec2i pos, const std::string &text, std::map<char, Character> &mapping, ShaderProgram *shader);

        void draw(Vec2f pos, const std::string &text, std::map<char, Character> &mapping, ShaderProgram *shader);

        void renderPresent();

    private:
        RenderDevice *renderDevice = nullptr;
    };
}

#endif //MANA_RENDERER2D_HPP
