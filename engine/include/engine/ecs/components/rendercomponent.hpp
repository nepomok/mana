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

#ifndef MANA_RENDERCOMPONENT_HPP
#define MANA_RENDERCOMPONENT_HPP

#include "engine/ecs/component.hpp"

#include "engine/script/script.hpp"

namespace mana {
    struct RenderComponent : public Component {
        RenderComponent() : Component(RENDER) {}

        Component *clone() override {
            return new RenderComponent(*this);
        }

        const std::type_info &getTypeInfo() override {
            return typeid(RenderComponent);
        }

        bool forward = false;

        bool outline;
        ColorRGBA outlineColor;
        float outlineScale = 1.1f;
    };
}

#endif //MANA_RENDERCOMPONENT_HPP
