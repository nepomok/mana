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

#ifndef MANA_LIGHTCOMPONENT_HPP
#define MANA_LIGHTCOMPONENT_HPP

#include "engine/ecs/component.hpp"

#include "engine/render/lighting/light.hpp"

namespace mana {
    struct LightComponent : public Component {
        LightComponent() : Component(LIGHT) {}

        LightType lightType = LIGHT_DIRECTIONAL;

        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);

        Vec3f direction;

        float cutOff = 10;
        float outerCutOff = 14;

        float constant = 1;
        float linear = 1;
        float quadratic = 1;
    };
}

#endif //MANA_LIGHTCOMPONENT_HPP
