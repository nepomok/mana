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

#ifndef MANA_RENDERER_HPP
#define MANA_RENDERER_HPP

#include "engine/asset/image.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/rendertarget.hpp"
#include "engine/render/meshbuffer.hpp"
#include "engine/render/texturebuffer.hpp"
#include "engine/render/graphicsbackend.hpp"

namespace engine {
    struct RenderOptions {
        explicit RenderOptions(Vec2i viewportOffset,
                               Vec2i viewportSize,
                               bool multiSample = false,
                               ColorRGBA clearColorValue = {},
                               bool clearColor = true,
                               bool clearDepth = true,
                               bool clearStencil = true)
                : viewportOffset(viewportOffset), viewportSize(viewportSize),
                  multiSample(multiSample),
                  clearColorValue(clearColorValue),
                  clearColor(clearColor),
                  clearDepth(clearDepth),
                  clearStencil(clearStencil) {}

        Vec2i viewportOffset;
        Vec2i viewportSize;
        bool multiSample;
        ColorRGBA clearColorValue;
        bool clearColor;
        bool clearDepth;
        bool clearStencil;
    };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void renderBegin(RenderTarget &target, const RenderOptions &options) = 0;

        virtual void addCommand(RenderCommand &command) = 0;

        virtual void renderFinish() = 0;
    };
}

#endif //MANA_RENDERER_HPP
