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

#include "engine/resource/json/jsonnoderesource.hpp"
#include "engine/resource/json/jsoncomponentresource.hpp"

#include "engine/resource/memory/memorystringresource.hpp"

#include "extern/json.hpp"

namespace mana {
    std::pair<std::string, Node> getNodeFromJson(const std::string &jsonStr) {
        std::pair<std::string, Node> ret;
        auto j = nlohmann::json::parse(jsonStr);
        ret.first = j["nodeName"];
        for (auto &component : j["components"]) {
            MemoryStringResource s(nlohmann::to_string(component));
            JsonComponentResource res(s);
            ret.second.addComponentPointer(res.getComponent());
        }
        return ret;
    }

    JsonNodeResource::JsonNodeResource(TextResource &jsonText)
            : jsonText(jsonText) {}

    JsonNodeResource::~JsonNodeResource() {}

    std::string JsonNodeResource::getName() {
        if (!isLoaded)
            load();
        return nodeName;
    }

    Node JsonNodeResource::getNode() {
        if (!isLoaded)
            load();
        return node;
    }

    void JsonNodeResource::load() {
        auto r = getNodeFromJson(jsonText.getText());
        nodeName = r.first;
        node = r.second;
        isLoaded = true;
    }

    void JsonNodeResource::free() {
        nodeName.clear();
        node.enabled = false;
        node.components.clear();
        isLoaded = false;
    }
}
