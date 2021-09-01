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

#include "engine/resource/resourcemanager.hpp"

#include <filesystem>

namespace mana {
    ResourceManager::ResourceManager(Archive &archive)
            : archive(archive), pool(1) {}

    void ResourceManager::loadAssetScene(const std::string &path) {
        std::lock_guard<std::mutex> guard(mutex);
        auto it = tasks.find(path);
        if (it == tasks.end()) {
            //Create task
            tasks[path] = pool.addTask([this, path]() {
                std::unique_ptr<std::iostream> source(archive.open(path));
                auto scene = AssetImporter::importAssetScene(*source, std::filesystem::path(path).extension());

                std::lock_guard<std::mutex> guard(mutex);
                scenes[path] = std::move(scene);
            });
        }
    }

    void ResourceManager::loadImage(const std::string &path) {
        std::lock_guard<std::mutex> guard(mutex);
        auto it = tasks.find(path);
        if (it == tasks.end()) {
            //Create task
            tasks[path] = pool.addTask([this, path]() {
                std::unique_ptr<std::iostream> source(archive.open(path));
                auto image = AssetImporter::importImage(*source, std::filesystem::path(path).extension());

                std::lock_guard<std::mutex> guard(mutex);
                images[path] = std::move(image);
            });
        }
    }

    void ResourceManager::loadAudio(const std::string &path) {
        std::lock_guard<std::mutex> guard(mutex);
        auto it = tasks.find(path);
        if (it == tasks.end()) {
            //Create task
            tasks[path] = pool.addTask([this, path]() {
                std::unique_ptr<std::iostream> source(archive.open(path));
                auto a = AssetImporter::importAudio(*source, std::filesystem::path(path).extension());

                std::lock_guard<std::mutex> guard(mutex);
                audio[path] = std::move(a);
            });
        }
    }

    void ResourceManager::unload(const std::string &path) {
        mutex.lock();

        auto it = tasks.find(path);
        if (it != tasks.end()) {
            auto ptr = it->second;
            mutex.unlock();
            ptr->wait();

            mutex.lock();

            tasks.erase(path);
            scenes.erase(path);
            images.erase(path);
            audio.erase(path);
        }

        mutex.unlock();
    }

    const AssetScene &ResourceManager::getAssetScene(const std::string &path) {
        mutex.lock();

        auto it = tasks.find(path);
        if (it == tasks.end()) {
            mutex.unlock();
            loadAssetScene(path);
            mutex.lock();
            it = tasks.find(path);
        }

        auto ptr = it->second;

        mutex.unlock();

        ptr->wait();

        mutex.lock();

        const AssetScene &ret = scenes.at(path);

        mutex.unlock();

        return ret;
    }

    const Image<ColorRGBA> &ResourceManager::getImage(const std::string &path) {
        mutex.lock();

        auto it = tasks.find(path);
        if (it == tasks.end()) {
            mutex.unlock();
            loadImage(path);
            mutex.lock();
            it = tasks.find(path);
        }

        auto ptr = it->second;

        mutex.unlock();

        ptr->wait();

        mutex.lock();

        const Image<ColorRGBA> &ret = images.at(path);

        mutex.unlock();

        return ret;
    }

    const Audio &ResourceManager::getAudio(const std::string &path) {
        mutex.lock();

        auto it = tasks.find(path);
        if (it == tasks.end()) {
            mutex.unlock();
            loadImage(path);
            mutex.lock();
            it = tasks.find(path);
        }

        auto ptr = it->second;

        mutex.unlock();

        ptr->wait();

        mutex.lock();

        const Audio &ret = audio.at(path);

        mutex.unlock();

        return ret;
    }
}