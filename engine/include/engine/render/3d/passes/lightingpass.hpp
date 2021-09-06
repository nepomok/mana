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

#ifndef ENGINE_LIGHTINGPASS_HPP
#define ENGINE_LIGHTINGPASS_HPP

#include "engine/render/3d/renderpass.hpp"

namespace engine {
    class LightingPass : public RenderPass {
    public:
        explicit LightingPass(RenderDevice &device);

        ~LightingPass() override = default;

        void render(RenderTarget &screen, RenderScene &scene, GeometryBuffer &gBuffer) override;

    private:
        RenderDevice &renderDevice;

        ShaderProgram *shader;
    };
}

#endif //ENGINE_LIGHTINGPASS_HPP
