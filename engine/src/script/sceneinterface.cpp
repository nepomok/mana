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

#include "sceneinterface.hpp"

#include <mutex>

namespace engine {
    namespace SceneInterface {
        std::mutex mutex;
        std::set<SceneInterfaceListener *> listeners;

        void addListener(SceneInterfaceListener *listener) {
            std::lock_guard<std::mutex> guard(mutex);
            listeners.insert(listener);
        }

        void removeListener(SceneInterfaceListener *listener) {
            std::lock_guard<std::mutex> guard(mutex);
            listeners.erase(listener);
        }

        extern "C"
        {
        void createNode(const char *name, const char *json) {
            std::lock_guard<std::mutex> guard(mutex);
            for (auto *listener : listeners)
                listener->createNode(name, json);
        }

        void destroyNode(const char *name) {
            std::lock_guard<std::mutex> guard(mutex);
            for (auto *listener : listeners)
                listener->destroyNode(name);
        }

        void createComponent(const char *node, const char *json) {
            std::lock_guard<std::mutex> guard(mutex);
            for (auto *listener : listeners)
                listener->createComponent(node, json);
        }

        void destroyComponent(const char *node, ComponentType type) {
            std::lock_guard<std::mutex> guard(mutex);
            for (auto *listener : listeners)
                listener->destroyComponent(node, type);
        }
        }
    }
}