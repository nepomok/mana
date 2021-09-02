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

#ifndef MANA_ASSETBUNDLE_HPP
#define MANA_ASSETBUNDLE_HPP

#include <map>
#include <string>

#include "engine/asset/mesh.hpp"
#include "engine/asset/image.hpp"
#include "engine/asset/material.hpp"
#include "engine/asset/texture.hpp"
#include "engine/asset/shader.hpp"

namespace mana {
    class AssetBundle {
    public:
        template<typename T>
        static const T &get(const std::string &name, const std::map<std::string, T> &map) {
            if (name.empty()) {
                if (map.empty())
                    throw std::runtime_error("Invalid name " + name);
                return map.begin()->second;
            } else {
                return map.at(name);
            }
        }

        const Mesh &getMesh(const std::string &name) const { return get<Mesh>(name, meshes); }

        const Material &getMaterial(const std::string &name) const { return get<Material>(name, materials); }

        const Image<ColorRGBA> &getImage(const std::string &name) const { return get<Image<ColorRGBA>>(name, images); }

        const Texture &getTexture(const std::string &name) const { return get<Texture>(name, textures); }

        const Audio &getAudio(const std::string &name) const { return get<Audio>(name, audio); }

        std::map<std::string, Mesh> meshes;
        std::map<std::string, Material> materials;
        std::map<std::string, Image<ColorRGBA>> images;
        std::map<std::string, Texture> textures;
        std::map<std::string, Audio> audio;
    };
}

#endif //MANA_ASSETBUNDLE_HPP
