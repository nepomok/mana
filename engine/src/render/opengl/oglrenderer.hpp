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

#ifndef MANA_OGLRENDERER_HPP
#define MANA_OGLRENDERER_HPP

#include "engine/render/renderer.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderer : public Renderer {
        public:
            void setCamera(const Camera &camera) override;

            void setViewport(Vec2i offset, Vec2i size) override;

            void setClear(bool clearColor, bool clearDepth, bool clearStencil) override;

            void setClearColor(ColorRGBA clearColor) override;

            void setMultiSample(bool multiSample) override;

            void setDirectionalLights(const std::vector<DirectionalLight> &lights) override;

            void setPointLights(const std::vector<PointLight> &lights) override;

            void setSpotLights(const std::vector<SpotLight> &lights) override;

            void renderBegin(const RenderTarget &target) override;

            void addCommand(const RenderCommand &command) override;

            void addCommands(const std::vector<RenderCommand> &commands) override;

            void renderFinish() override;

        private:
            Vec2i viewportOffset = {}, viewportSize = {};
            ColorRGBA clearColorValue = {};
            bool clearColor = true, clearDepth = true, clearStencil = true;
            bool multiSample = false;

            Mat4f view, projection;
            Transform cameraTransform;
            std::vector<DirectionalLight> directionalLights;
            std::vector<PointLight> pointLights;
            std::vector<SpotLight> spotLights;
        };
    }
}

#endif //MANA_OGLRENDERER_HPP
