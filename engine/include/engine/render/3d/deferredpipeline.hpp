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

#ifndef MANA_DEFERREDPIPELINE_HPP
#define MANA_DEFERREDPIPELINE_HPP

#include "engine/render/rendertarget.hpp"
#include "engine/render/renderer.hpp"

#include "engine/render/3d/renderscene.hpp"
#include "engine/render/3d/renderpass.hpp"

namespace mana {
    class DeferredPipeline {
    public:
        DeferredPipeline() = default;

        /**
         * The object takes ownership of the render pass pointers.
         *
         * @param ren
         * @param passes The render passes to use in this pipeline. The pipeline destructor deletes the passes.
         */
        DeferredPipeline(RenderDevice &device, std::vector<RenderPass *> passes);

        ~DeferredPipeline();

        /**
         * The screen target contains depth and color information of the deferred commands after this method returns.
         *
         * @param screen
         * @param scene
         */
        void render(RenderTarget &screen, RenderScene &scene);

    private:
        RenderDevice *renderDevice{};
        GeometryBuffer gBuffer;
        std::vector<RenderPass *> passes;
    };
}

#endif //MANA_DEFERREDPIPELINE_HPP
