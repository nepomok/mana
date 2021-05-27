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
#include <map>
#include <functional>

#include "engine/render/renderer.hpp"
#include "engine/render/renderdevice.hpp"

#include "engine/render/3d/renderscene.hpp"
#include "engine/render/3d/renderpass.hpp"

namespace mana {
    class Renderer3D {
    public:
        static const std::map<std::string, std::string> &getShaderMacros();

        static const std::function<std::string(const char *)> &getShaderIncludeCallback();

        Renderer3D();

        Renderer3D(RenderDevice &device, std::vector<RenderPass *> passes);

        Renderer3D(Renderer3D &&other) noexcept;

        ~Renderer3D();

        Renderer3D &operator=(Renderer3D &&other) noexcept ;

        void setRenderDevice(RenderDevice *device);

        const RenderDevice &getRenderDevice();

        void setRenderPasses(std::vector<RenderPass *> passes);

        const std::vector<RenderPass *> &getRenderPasses();

        void render(RenderTarget &target,
                    RenderScene &scene);

    private:
        RenderDevice *device;
        std::vector<RenderPass *> passes;
    };
}

#endif //MANA_RENDERER3D_HPP
