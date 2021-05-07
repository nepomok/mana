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

#include "engine/render/2d/renderer2d.hpp"

namespace mana {
    Renderer2D::Renderer2D() {

    }

    Renderer2D::Renderer2D(Renderer &ren, RenderAllocator &alloc) {

    }

    void Renderer2D::setEnableAlphaBlending(bool enable) {

    }

    void Renderer2D::renderBegin(const RenderTarget &target) {

    }

    void Renderer2D::renderBegin(const RenderTarget &target, Vec2i viewportOffset, Vec2i viewportSize) {

    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture, const ShaderProgram &shader,
                          Vec2f center, float rotation) {

    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture, const ShaderProgram &shader) {

    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture, Vec2f center, float rotation) {

    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, const TextureBuffer &texture) {

    }

    void Renderer2D::draw(Rectf rectangle, ColorRGBA color, bool fill, Vec2f center, float rotation) {

    }

    void Renderer2D::draw(Rectf rectangle, ColorRGBA color, bool fill) {

    }

    void Renderer2D::draw(Vec2f start, Vec2f end, ColorRGBA color, Vec2f center, float rotation) {

    }

    void Renderer2D::draw(Vec2f start, Vec2f end, ColorRGBA color) {

    }

    void Renderer2D::draw(Vec2f point, ColorRGBA color) {

    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture, const ShaderProgram &shader,
                          Vec2i center, float rotation) {

    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture, const ShaderProgram &shader) {

    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture, Vec2i center, float rotation) {

    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, const TextureBuffer &texture) {

    }

    void Renderer2D::draw(Recti rectangle, ColorRGBA color, bool fill, Vec2i center, float rotation) {

    }

    void Renderer2D::draw(Recti rectangle, ColorRGBA color, bool fill) {

    }

    void Renderer2D::draw(Vec2i start, Vec2i end, ColorRGBA color, Vec2i center, float rotation) {

    }

    void Renderer2D::draw(Vec2i start, Vec2i end, ColorRGBA color) {

    }

    void Renderer2D::draw(Vec2i point, ColorRGBA color) {

    }

    void Renderer2D::renderPresent() {

    }
}