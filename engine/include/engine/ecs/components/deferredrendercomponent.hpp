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

#ifndef MANA_DEFERREDRENDERCOMPONENT_HPP
#define MANA_DEFERREDRENDERCOMPONENT_HPP

#include "engine/ecs/component.hpp"

#include "engine/render/rendercommand.hpp"
#include "engine/render/3d/material.hpp"

#include "engine/resource/resourcehandle.hpp"

namespace mana {
    struct DeferredRenderComponent : public Component {
        DeferredRenderComponent() : Component(RENDER_DEFERRED) {}

        Component *clone() override {
            return new DeferredRenderComponent(*this);
        }

        const std::type_info &getTypeInfo() override {
            return typeid(DeferredRenderComponent);
        }

        ResourceHandle<MeshBuffer> meshBuffer;
        ResourceHandle<Material> material;

        bool outline = false;
        ColorRGB outlineColor;
        float outlineScale = 1.1f;
    };
}

#endif //MANA_DEFERREDRENDERCOMPONENT_HPP