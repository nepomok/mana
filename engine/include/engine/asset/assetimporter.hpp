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

#ifndef MANA_ASSETIMPORTER_HPP
#define MANA_ASSETIMPORTER_HPP

#include <string>
#include <vector>

#include "engine/io/archive.hpp"

#include "engine/asset/material.hpp"
#include "engine/asset/audio.hpp"
#include "engine/asset/assetbundle.hpp"

#include "engine/async/threadpool.hpp"

namespace engine {
    /**
     * The asset importer imports bundles.
     *
     * A asset bundle is the smallest unit loaded by the importer.
     *
     * The user can configure how much data is loaded at once through the asset bundle mechanism.
     *
     * A user can for example specify each fbx as a bundle in the scene in which case the fbx is loaded only
     * when a component references the fbx directly.
     *
     * Alternatively the user can define a json bundle which includes multiple fbx files. When this bundle
     * is loaded all referenced assets are loaded into memory as long as json bundle is referenced in some component.
     */
    class AssetImporter {
    public:
        explicit AssetImporter(Archive &archive);

        /**
         * Import asset bundle identified by path asynchronously.
         *
         * @param path
         */
        void import(const std::string &path);

        void clear(const std::string &path);

        const AssetBundle &getBundle(const std::string &path);

    private:
        Archive &archive;

        std::mutex mutex;
        std::map<std::string, std::shared_ptr<Task>> tasks;
        std::map<std::string, AssetBundle> bundles;
    };
}

#endif //MANA_ASSETIMPORTER_HPP
