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

#ifndef MANA_CAMERACOMPONENT_HPP
#define MANA_CAMERACOMPONENT_HPP

#include "engine/ecs/component.hpp"

#include "engine/render/3d/camera/camera.hpp"

namespace mana {
    struct CameraComponent : public Component {
        CameraComponent() : Component(CAMERA) {}

        Component *clone() override {
            return new CameraComponent(*this);
        }

        const std::type_info& getTypeInfo() override {
            return typeid(CameraComponent);
        }

        CameraType cameraType = PERSPECTIVE;

        float nearClip = 0.1f;
        float farClip = 1000.0f;

        float left = -10;
        float top = 10;
        float right = 10;
        float bottom = -10;

        float fov = 60;
        float aspectRatio = 4.0f / 3.0f;
    };
}

#endif //MANA_CAMERACOMPONENT_HPP
