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

#include "engine/resource/json/jsonsceneresource.hpp"

#include "engine/resource/memory/memorystringresource.hpp"
#include "engine/resource/json/jsonnoderesource.hpp"

#include "extern/json.hpp"

namespace mana {
    std::pair<std::string, Scene> getSceneFromJson(const std::string &jsonStr) {
        auto j = nlohmann::json::parse(jsonStr);
        std::string name;
        Scene scene;
        for (auto &node : j["nodes"]) {
            MemoryStringResource res(nlohmann::to_string(node));
            JsonNodeResource n(res);
            scene.nodes[n.getName()] = n.getNode();
        }
        return {name, scene};
    }

    JsonSceneResource::JsonSceneResource(TextResource &jsonText)
            : jsonText(jsonText) {
    }

    JsonSceneResource::~JsonSceneResource() = default;

    const std::string &JsonSceneResource::getName() {
        if (!isLoaded)
            load();
        return name;
    }

    Scene JsonSceneResource::getScene() {
        if (!isLoaded)
            load();
        return scene;
    }

    void JsonSceneResource::load() {
        auto r = getSceneFromJson(jsonText.getText());
        name = r.first;
        scene = r.second;
        isLoaded = true;
    }

    void JsonSceneResource::free() {
        name = {};
        scene = {};
        isLoaded = false;
    }
}