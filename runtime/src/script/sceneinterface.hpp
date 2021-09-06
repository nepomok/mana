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

#ifndef MANA_SCENEINTERFACE_HPP
#define MANA_SCENEINTERFACE_HPP

namespace engine {
    namespace SceneInterface {
        class SceneInterfaceListener {
        public:
            virtual void createNode(const char *name, const char *json) {}

            virtual void destroyNode(const char *name) {}

            virtual void createComponent(const char *node, const char *json) {}

            virtual void destroyComponent(const char *node, ComponentType type) {}
        };

        void addListener(SceneInterfaceListener *listener);

        void removeListener(SceneInterfaceListener *listener);

        extern "C"
        {
        void createNode(const char *name, const char *json);

        void destroyNode(const char *name);

        void createComponent(const char *node, const char *json);

        void destroyComponent(const char *node, ComponentType type);
        }
    }
}

#endif //MANA_SCENEINTERFACE_HPP
