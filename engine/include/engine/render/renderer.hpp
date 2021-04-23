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

#ifndef MANA_RENDERER_HPP
#define MANA_RENDERER_HPP

#include "engine/render/imagebuffer.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/framebuffer.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/renderscene.hpp"

namespace mana {
    class Renderer {
    public:
        virtual void render(const FrameBuffer &frameBuffer,
                            const RenderScene &scene,
                            Vec2i viewportOffset,
                            Vec2i viewportSize,
                            ColorRGBA clearColorValue,
                            bool clearColor,
                            bool clearDepth,
                            bool clearStencil,
                            bool multiSample) = 0;

        virtual void render(const FrameBuffer &frameBuffer,
                            const RenderScene &scene,
                            ColorRGBA clearColorValue = ColorRGBA(0),
                            bool clearColor = true,
                            bool clearDepth = true,
                            bool clearStencil = true,
                            bool multiSample = true) = 0;
    };
}

#endif //MANA_RENDERER_HPP
