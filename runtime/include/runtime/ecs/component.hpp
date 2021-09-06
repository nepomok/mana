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

#ifndef MANA_COMPONENT_HPP
#define MANA_COMPONENT_HPP

#include "componenttype.hpp"

#include <memory>

namespace engine::runtime {
    struct Node;

    /**
     * A Component provides static state.
     * This state should be as descriptive as possible and be static.
     *
     * The requirement is that components can be serialized to and from some format without loosing state.
     *
     * So components cannot for example store a pointer to some dynamically allocated object and instead
     * describe the object in serializable form such as asset paths and properties and the systems
     * then figure out at runtime how and when to allocate the object.
     *
     * For shareable data such as meshes and textures asset paths are used which allows systems to efficiently
     * allocate the objects.
     *
     * For non shareable data such as a script instance the systems use the userData interface.
     *
     * Systems cannot assume that the scene state persists after going out of scope of the update method.
     *
     * Therefore a managed pointer is used for the userData which enables UserData subclasses to handle when the component
     * is destroyed.
     */
    struct Component {
        class UserData {
        public:
            virtual ~UserData() = default;
        };

        /**
         * This managed pointer stores data which is unique to this component and is allocated at runtime by a system.
         * For example a script instance would be stored in user data or a rigidbody instance,
         * and the systems allocate the user data if it isnt allocated.
         *
         * This way the scene data remains independent of the systems state.
         */
        std::unique_ptr<UserData> userData = nullptr;

        bool enabled = true;
        Node *node = nullptr;

        const ComponentType componentType;

        explicit Component(ComponentType type) : componentType(type) {}

        virtual ~Component() = default;

        Component(const Component &copy)
                : componentType(copy.componentType) {
            userData = nullptr;
        }

        virtual Component *clone() = 0;

        virtual const std::type_info &getTypeInfo() = 0;
    };
}

#endif //MANA_COMPONENT_HPP
