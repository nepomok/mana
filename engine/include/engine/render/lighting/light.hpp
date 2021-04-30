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

#ifndef MANA_LIGHT_HPP
#define MANA_LIGHT_HPP

#include "engine/math/transform.hpp"

namespace mana {
    enum LightType {
        LIGHT_DIRECTIONAL,
        LIGHT_POINT,
        LIGHT_SPOT
    };

    struct Light {
        explicit Light(LightType type) : type(type) {}

        LightType type;
        Transform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
    };

    struct DirectionalLight : public Light {
        DirectionalLight() : Light(LIGHT_DIRECTIONAL) {}

        Vec3f direction;
    };

    struct PointLight : public Light {
        PointLight() : Light(LIGHT_POINT) {}

        float constant = 1;
        float linear = 1;
        float quadratic = 1;
    };

    struct SpotLight : public Light {
        SpotLight() : Light(LIGHT_SPOT) {}

        Vec3f direction = Vec3f(0, -1, 0);
        float cutOff = 10;
        float outerCutOff = 14;
        float constant = 0.1;
        float linear = 0.1;
        float quadratic = 0.1;
    };
}

#endif //MANA_LIGHT_HPP
