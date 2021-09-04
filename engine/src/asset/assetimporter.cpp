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

#include "engine/asset/assetimporter.hpp"

#include "engine/asset/assetloader.hpp"

#include "../extern/json.hpp"

#include <filesystem>
#include <sstream>

namespace mana {
    AssetImporter::AssetImporter(Archive &archive)
            : archive(archive) {}

    void AssetImporter::import(const std::string &path) {
        std::lock_guard<std::mutex> guard(mutex);

        auto bundleIterator = bundles.find(path);
        if (bundleIterator == bundles.end()) {
            auto taskIterator = tasks.find(path);
            if (taskIterator == tasks.end()) {
                //Create load task
                tasks[path] = ThreadPool::pool.addTask([this, path]() {
                    AssetBundle bundle;
                    std::string hint = std::filesystem::path(path).extension();

                    std::unique_ptr<std::iostream> stream(archive.open(path));
                    bundle = AssetLoader::loadBundle(*stream, hint, archive, ThreadPool::pool);

                    std::lock_guard<std::mutex> guard(mutex);
                    bundles[path] = bundle;

                    //The pool holds a shared ptr to this task so it will get deleted after the task returns.
                    tasks.erase(path);
                });
            }
        }
    }

    void AssetImporter::clear(const std::string &path) {
        mutex.lock();

        auto it = tasks.find(path);
        if (it != tasks.end()) {
            auto task = it->second;
            mutex.unlock();
            task->wait();
            mutex.lock();
        }

        bundles.erase(path);

        mutex.unlock();
    }

    const AssetBundle &AssetImporter::getBundle(const std::string &path) {
        mutex.lock();

        auto bundle = bundles.find(path);
        auto task = tasks.find(path);
        if (bundle == bundles.end() && task == tasks.end()) {
            mutex.unlock();
            import(path);
            mutex.lock();
        } else if (task != tasks.end()) {
            auto taskCopy = task->second;
            mutex.unlock();
            taskCopy->wait();
            mutex.lock();
        }

        task = tasks.find(path);
        if (task != tasks.end()) {
            auto taskCopy = task->second;
            mutex.unlock();
            taskCopy->wait();
            mutex.lock();
        }

        const AssetBundle &ret = bundles.at(path);

        mutex.unlock();

        return ret;
    }
}