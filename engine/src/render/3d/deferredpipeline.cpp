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

#include "engine/render/3d/deferredpipeline.hpp"

namespace engine {
    DeferredPipeline::DeferredPipeline(RenderDevice &device, std::vector<RenderPass *> passes)
            : renderDevice(&device),
              passes(std::move(passes)),
              gBuffer(device.getAllocator()) {}

    DeferredPipeline::~DeferredPipeline() {
        for (auto *p : passes)
            delete p;
    }

    void DeferredPipeline::render(RenderTarget &screen, RenderScene &scene) {
        gBuffer.setSize(screen.getSize());

        for (auto *pass : passes) {
            pass->render(screen, scene, gBuffer);
        }

        // Blit depth from geometry buffer to screen target, the geometry buffer target contains the correct
        // scene depth, color is written directly to the screen by the passes.
        screen.blitDepth(gBuffer.getRenderTarget(), {}, {}, gBuffer.getSize(), screen.getSize());
    }
}