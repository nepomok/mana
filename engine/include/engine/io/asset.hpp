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

#ifndef MANA_ASSET_HPP
#define MANA_ASSET_HPP

#include <map>
#include <string>

#include "engine/asset/mesh.hpp"
#include "engine/render/renderdevice.hpp"
#include "engine/render/3d/rendermaterial.hpp"

namespace mana {
    // No embedded textures / texture paths support for now
    struct AssetMaterial {
        std::string name{};

        ColorRGBA diffuseColor{};
        ColorRGBA ambientColor{};
        ColorRGBA specularColor{};
        float shininess{};

        RenderMaterial createMaterial(RenderDevice &dev) const {
            RenderMaterial ret;
            ret.diffuse = diffuseColor;
            ret.ambient = ambientColor;
            ret.specular = specularColor;
            ret.shininess = shininess;

            return ret;
        }
    };

    class Asset {
    public:
        virtual const std::map<std::string, Mesh> &getMeshes() = 0;

        virtual const std::map<std::string, AssetMaterial> &getMaterials() = 0;
    };
}
#endif //MANA_ASSET_HPP
