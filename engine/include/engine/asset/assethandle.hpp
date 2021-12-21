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

#ifndef MANA_ASSETHANDLE_HPP
#define MANA_ASSETHANDLE_HPP

#include "engine/asset/manager/assetmanager.hpp"
#include "engine/asset/manager/assetrendermanager.hpp"

namespace engine {
    template<typename T>
    class AssetHandle {
    public:
        AssetHandle() = default;

        AssetHandle(const AssetPath &path, AssetManager &manager, AssetRenderManager *renderManager)
                : path(path), manager(&manager), renderManager(renderManager) {
            manager.incrementRef(path);
            if (renderManager != nullptr)
                renderManager->incrementRef(path);
        }

        ~AssetHandle() {
            if (!path.empty()) {
                if (manager != nullptr)
                    manager->decrementRef(path);
                if (renderManager != nullptr)
                    renderManager->decrementRef<T>(path);
            }
        }

        AssetHandle(const AssetHandle<T> &other) {
            *this = other;
        }

        AssetHandle<T> &operator=(const AssetHandle<T> &other) {
            if (this == &other)
                return *this;

            path = other.path;
            manager = other.manager;
            renderManager = other.renderManager;
            if (!path.empty()) {
                if (manager != nullptr)
                    manager->incrementRef(path);
                if (renderManager != nullptr)
                    renderManager->incrementRef(path);
            }

            return *this;
        }

        const T &get() {
            if (renderManager == nullptr)
                throw std::runtime_error("nullptr dereference");
            return manager->getAsset<T>(path);
        }

        template<typename R>
        R &getRenderObject() {
            if (renderManager == nullptr)
                throw std::runtime_error("nullptr dereference");
            return renderManager->get<R>(path);
        }

        const AssetPath &getPath() const {
            return path;
        }

        bool isAssigned() const {
            return !path.empty();
        }

    private:
        AssetPath path;
        AssetManager *manager = nullptr;
        AssetRenderManager *renderManager = nullptr;
    };
}

#endif //MANA_ASSETHANDLE_HPP