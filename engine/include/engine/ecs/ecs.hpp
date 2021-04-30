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

#ifndef MANA_ECS_HPP
#define MANA_ECS_HPP

#include "engine/ecs/system.hpp"

namespace mana {
    class ECS {
    public:
        ECS();

        explicit ECS(Scene scene);

        ~ECS();

        void setScene(Scene scene);

        Scene &getScene();

        void addSystem(System *system);

        void removeSystem(System *system);

        void update(float deltaTime);

    private:
        Scene scene;

        std::set<System *> systems;
    };
}

#endif //MANA_ECS_HPP
