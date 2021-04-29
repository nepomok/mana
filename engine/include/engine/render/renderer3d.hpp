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

#ifndef MANA_RENDERER3D_HPP
#define MANA_RENDERER3D_HPP

#include "engine/render/renderer.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    class Renderer3D {
    public:
        static std::string preprocessGlsl(std::string shader);

        Renderer3D();

        Renderer3D(Renderer &ren, RenderAllocator &alloc);

        void setCamera(const Camera &camera);

        void setDirectionalLights(const std::vector<DirectionalLight> &lights);

        void setPointLights(const std::vector<PointLight> &lights);

        void setSpotLights(const std::vector<SpotLight> &lights);

        void setEnableShadowMapping(bool shadowMapping);

        void draw(const RenderTarget &target, const std::vector<RenderCommand> &commands);

    private:
        Renderer *ren;
        RenderAllocator *alloc;

        const Camera *camera;

        std::vector<DirectionalLight> dir;
        std::vector<PointLight> point;
        std::vector<SpotLight> spot;
    };
}

#endif //MANA_RENDERER3D_HPP
