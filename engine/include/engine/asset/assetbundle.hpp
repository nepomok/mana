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

namespace engine {
    class AssetBundle {
    public:
        ~AssetBundle() {
            for (auto &pair: assets)
                delete pair.second;
        }

        AssetBundle() = default;

        AssetBundle(const AssetBundle &other) {
            *this = other;
        }

        AssetBundle &operator=(const AssetBundle &other) {
            for (auto &pair: other.assets)
                assets.insert({pair.first, pair.second->clone()});
            return *this;
        }

        AssetBundle(AssetBundle &&other) = default;

        AssetBundle &operator=(AssetBundle &&other) = default;

        template<typename T>
        const T &get(const std::string &name = "") const {
            if (assets.empty())
                throw std::runtime_error("Empty bundle map");
            if (name.empty()) {
                return dynamic_cast<const T &>(*assets.begin()->second);
            } else {
                return dynamic_cast<const T &>(*assets.at(name));
            }
        }

        void add(const std::string &name, Asset *asset) {
            if (assets.find(name) != assets.end())
                throw std::runtime_error("Asset with name " + name + " already exists");
            assets[name] = asset;
        }

        void remove(const std::string &name) {
            delete assets.at(name);
            assets.erase(name);
        }

        std::map<std::string, Asset *> assets;
    };
}

#endif //MANA_ASSETBUNDLE_HPP
