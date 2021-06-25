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

#include "engine/ecs/scene.hpp"
#include "engine/ecs/components.hpp"

#include "script/sceneinterface.hpp"

namespace mana {
    namespace SceneInterface {
        Component *createInstance(ComponentType type) {
            switch (type) {
                case TRANSFORM:
                    return new TransformComponent();
                case CAMERA:
                    return new CameraComponent();
                case RENDER:
                    return new RenderComponent();
                case LIGHT:
                    return new LightComponent();
                case SCRIPT:
                    return new ScriptComponent();
                default:
                    throw std::runtime_error("Unrecognized component type " + std::to_string(type));
            }
        }

        Scene *scene = nullptr;

        void setScene(Scene *s) {
            scene = s;
        }

        extern "C"
        {
        void createNode(const char *name) {
            assert(scene != nullptr);
            scene->nodes[name] = Node();
        }

        void destroyNode(const char *name) {
            assert(scene != nullptr);
            scene->nodes.erase(name);
        }

        void createComponent(const char *nodeName, ComponentType type) {
            assert(scene != nullptr);
            (*scene)[nodeName].components.insert({Node::getComponentTypeIndex(type), createInstance(type)});
        }

        void destroyComponent(const char *nodeName, ComponentType type) {
            assert(scene != nullptr);
            (*scene)[nodeName].components.erase(Node::getComponentTypeIndex(type));
        }
        }
    }
}