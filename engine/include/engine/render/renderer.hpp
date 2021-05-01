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

#include "engine/render/imagebuffer.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/rendertarget.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/camera/camera.hpp"
#include "engine/render/graphicsapi.hpp"
#include "engine/render/lighting/light.hpp"

namespace mana {
    /**
    * The renderer uses right handed coordinate system when handling render command transforms.
    */
    class Renderer {
    public:
        virtual void setViewport(Vec2i offset, Vec2i size) = 0;

        virtual void setClear(bool clearColor, bool clearDepth, bool clearStencil) = 0;

        virtual void setClearColor(ColorRGBA clearColor) = 0;

        virtual void setMultiSample(bool multiSample) = 0;

        virtual void renderBegin(const RenderTarget &target) = 0;

        virtual void addCommand(const RenderCommand &command) = 0;
        
        virtual void renderFinish() = 0;
    };
}

#endif //MANA_RENDERER_HPP
