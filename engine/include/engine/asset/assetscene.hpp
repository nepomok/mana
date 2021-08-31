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

#ifndef MANA_ASSETSCENE_HPP
#define MANA_ASSETSCENE_HPP

#include <map>
#include <string>

#include "engine/asset/mesh.hpp"
#include "engine/asset/image.hpp"
#include "engine/asset/material.hpp"

namespace mana {
    /**
     * An asset scene which is import from 3d formats such as fbx or obj.
     * Can contain meshes, materials, bones, lights etc.
     *
     * The scene has to specify which elements of an asset scene are linked.
     * Relations defined in the file format are not supported.
     */
    class AssetScene {
    public:
        std::map<std::string, Mesh> meshes;
        std::map<std::string, Material> materials;
    };
}

#endif //MANA_ASSETSCENE_HPP
