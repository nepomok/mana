/**
 *  Mana - 3D Engine
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
    class Renderer2D {
    public:
        Renderer2D();

        Renderer2D(Renderer &ren, RenderAllocator &alloc);

        void begin(const RenderTarget &target);

        void draw(Recti rectangle);

        void present();

    private:
        Renderer *ren = nullptr;
        RenderAllocator *alloc = nullptr;
    };
}

#endif //MANA_RENDERER2D_HPP
