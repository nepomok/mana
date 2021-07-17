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

#include "engine/resource/file/meshfileresource.hpp"

#include <stdexcept>

#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mana {
    MeshFileResource::MeshFileResource() : assetFile(), meshName(), mesh() {}

    MeshFileResource::MeshFileResource(AssetFile assetFile, std::string meshName) : assetFile(std::move(assetFile)),
                                                                                    meshName(std::move(meshName)),
                                                                                    mesh() {}

    MeshFileResource::~MeshFileResource() {
        MeshFileResource::free();
    }

    void MeshFileResource::load() {
        if (loaded)
            return;
        mesh = assetFile.getMeshes().at(meshName);
        loaded = true;
    }

    void MeshFileResource::free() {
        if (!loaded)
            return;
        mesh = {};
        loaded = false;
    }

    Mesh &MeshFileResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return mesh;
    }

    bool MeshFileResource::isLoaded() {
        return loaded;
    }

    bool MeshFileResource::supportAsync() {
        return false;
    }

    Mesh &MeshFileResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}