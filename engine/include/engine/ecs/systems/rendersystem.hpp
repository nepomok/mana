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

#ifndef MANA_RENDERSYSTEM_HPP
#define MANA_RENDERSYSTEM_HPP

#include "engine/ecs/system.hpp"

#include "engine/render/renderer3d.hpp"
#include "engine/render/rendertarget.hpp"

namespace mana {
    class RenderSystem : public System {
    public:
        explicit RenderSystem(const RenderTarget &screenTarget, Renderer3D &ren);

        ~RenderSystem() override = default;

        void start() override;

        void stop() override;

        void update(float deltaTime, Scene &scene) override;

        void onNodeCreated(Node &node) override;

        void onComponentCreated(Node &node, Component &component) override;

        void onNodeRemoved(Node &node) override;

        void onComponentRemoved(Node &node, Component &component) override;

    private:
        const RenderTarget &screenTarget;
        Renderer3D &ren;
    };
}

#endif //MANA_RENDERSYSTEM_HPP
