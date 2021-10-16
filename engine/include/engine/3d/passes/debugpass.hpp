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

#ifndef MANA_DEBUGPASS_HPP
#define MANA_DEBUGPASS_HPP

#include "engine/3d/renderpass.hpp"

namespace engine {
    class DebugPass : public RenderPass {
    public:
        explicit DebugPass(RenderDevice &device);

        ~DebugPass() override;

        void prepareBuffer(GeometryBuffer &gBuffer) override;

        void render(GeometryBuffer &gBuffer, Scene &scene) override;

        void setDrawNormals(bool drawNormals);

        void setDrawLightCasters(bool drawLights);

    private:
        RenderDevice &device;
        ShaderProgram *shader;
        ShaderProgram *shaderLight;
        MeshBuffer *meshBuffer;

        bool drawNormals = false;
        bool drawLights = false;
    };
}

#endif //MANA_DEBUGPASS_HPP
