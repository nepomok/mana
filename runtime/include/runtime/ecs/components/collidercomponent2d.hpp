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

#ifndef MANA_COLLIDERCOMPONENT2D_HPP
#define MANA_COLLIDERCOMPONENT2D_HPP

#include "../component.hpp"

#include "engine/math/vector2.hpp"

namespace mana {
    struct ColliderComponent2D : public Component {
        ColliderComponent2D() : Component(COLLIDER_2D) {}

        Component *clone() override {
            return new ColliderComponent2D(*this);
        }

        const std::type_info &getTypeInfo() override {
            return typeid(ColliderComponent2D);
        }

        std::vector<Vec2f> shape;
    };
}

#endif //MANA_COLLIDERCOMPONENT2D_HPP
