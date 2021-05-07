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

#ifndef MANA_RENDERER3D_HPP
#define MANA_RENDERER3D_HPP

#include <utility>

#include "engine/render/renderer.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    class Renderer3D {
    public:
        struct Unit {
            Unit() : transform(), command() {}

            Unit(Transform t, RenderCommand command) : transform(t), command(std::move(command)) {}

            Transform transform;
            RenderCommand command;

            //If true the renderer3d will outline the unit in the resulting render with the supplied color or optional shader.
            bool outline = false;
            ColorRGB outlineColor;
            ShaderProgram *outlineShader = nullptr;
            float outlineScale = 1.1f;
        };

        struct RenderScene {
            Camera *camera;
            std::vector<Unit> units;
            std::vector<DirectionalLight> dir;
            std::vector<PointLight> point;
            std::vector<SpotLight> spot;
        };

        static const std::map<std::string, std::string> &getShaderMacros();

        static const std::function<std::string(const char *)> &getShaderIncludeCallback();

        Renderer3D();

        Renderer3D(Renderer &ren, RenderAllocator &alloc);

        void setEnableShadowMapping(bool shadowMapping);

        void render(const RenderTarget &target,
                    const RenderScene &scene);

        const Renderer &getRenderer();

        const RenderAllocator &getAllocator();

    private:
        Renderer *ren;
        RenderAllocator *alloc;

        ShaderProgram *singleColorShader;
    };
}

#endif //MANA_RENDERER3D_HPP
