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

#include "engine/resource/render/meshbufferresource.hpp"

#include <utility>

namespace mana {
    MeshBufferResource::MeshBufferResource()
            : device(nullptr),
              meshResource(),
              manager(nullptr),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::MeshBufferResource(RenderDevice &alloc,
                                           std::string meshResource,
                                           ResourceManager *manager)
            : device(&alloc),
              meshResource(std::move(meshResource)),
              manager(manager),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::MeshBufferResource(RenderDevice &alloc,
                                           std::string meshResource,
                                           ResourceManager *manager,
                                           std::vector<Transform> instanceOffsets)
            : device(&alloc),
              meshResource(std::move(meshResource)),
              manager(manager),
              instanceOffsets(std::move(instanceOffsets)),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::~MeshBufferResource() {
        MeshBufferResource::free();
    }

    void MeshBufferResource::load() {
        if (loaded)
            return;

        manager->incrementRef(meshResource);

        auto *meshRes = manager->getResource<Mesh>(meshResource);

        if (instanced)
            mesh = device->createInstancedMeshBuffer(meshRes->get(), instanceOffsets);
        else
            mesh = device->createMeshBuffer(meshRes->get());

        loaded = true;
    }

    void MeshBufferResource::free() {
        if (!loaded)
            return;
        delete mesh;
        mesh = nullptr;
        manager->decrementRef(meshResource);
        loaded = false;
    }

    MeshBuffer &MeshBufferResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return *mesh;
    }

    bool MeshBufferResource::isLoaded() {
        return loaded;
    }

    bool MeshBufferResource::supportAsync() {
        return false;
    }

    MeshBuffer &MeshBufferResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}