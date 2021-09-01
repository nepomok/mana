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

#ifndef MANA_RESOURCEMANAGER_HPP
#define MANA_RESOURCEMANAGER_HPP

#include "engine/io/archive.hpp"
#include "engine/asset/assetimporter.hpp"

#include "engine/async/threadpool.hpp"

namespace mana {
    class ResourceManager {
    public:
        ResourceManager(Archive &archive);

        void loadAssetScene(const std::string &path);

        void loadImage(const std::string &path);

        void loadAudio(const std::string &path);

        void unload(const std::string &path);

        const AssetScene &getAssetScene(const std::string &path);

        const Image<ColorRGBA> &getImage(const std::string &path);

        const Audio &getAudio(const std::string &path);

    private:
        Archive &archive;

        ThreadPool pool;

        std::mutex mutex;
        std::map<std::string, std::shared_ptr<Task>> tasks;

        std::map<std::string, AssetScene> scenes;
        std::map<std::string, Image<ColorRGBA>> images;
        std::map<std::string, Audio> audio;
    };
}

#endif //MANA_RESOURCEMANAGER_HPP
