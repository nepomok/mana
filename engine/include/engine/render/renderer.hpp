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

#include "engine/common/imagebuffer.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/rendertarget.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/renderscene.hpp"
#include "engine/render/graphicsapi.hpp"

namespace mana {
    class Renderer {
    public:
        virtual void setTarget(const RenderTarget &target) = 0;

        virtual void setScene(const RenderScene &scene) = 0;

        virtual void setViewport(Vec2i offset, Vec2i size) = 0;

        virtual void setClear(bool clearColor, bool clearDepth, bool clearStencil) = 0;

        virtual void setClearColor(ColorRGBA clearColor) = 0;

        virtual void setMultiSample(bool multiSample) = 0;

        virtual void render() = 0;
    };
}

#endif //MANA_RENDERER_HPP
