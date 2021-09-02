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

#include "engine/io/json/ecs/componentdeserializer.hpp"

#include <cassert>

#include "extern/json.hpp"

#include "engine/ecs/components.hpp"

#include "engine/render/rendercommand.hpp"

namespace mana {
    ComponentType convertComponentType(const std::string &str) {
        if (str == "transform")
            return TRANSFORM;
        else if (str == "camera")
            return CAMERA;
        else if (str == "light")
            return LIGHT;
        else if (str == "script")
            return SCRIPT;
        else if (str == "mesh_render")
            return MESH_RENDER;
        else if (str == "skybox")
            return SKYBOX;
        throw std::runtime_error("Invalid component type " + str);
    }

    CameraType convertCameraType(const std::string &str) {
        if (str == "perspective")
            return PERSPECTIVE;
        else if (str == "orthographic")
            return ORTHOGRAPHIC;
        throw std::runtime_error("Invalid camera type " + str);
    }

    LightType convertLightType(const std::string &str) {
        if (str == "directional")
            return LIGHT_DIRECTIONAL;
        else if (str == "point")
            return LIGHT_POINT;
        else if (str == "spot")
            return LIGHT_SPOT;
        throw std::runtime_error("Invalid light type " + str);
    }

    Transform convertTransform(const nlohmann::json &component) {
        Transform ret;

        ret.position.x = component["position"]["x"];
        ret.position.y = component["position"]["y"];
        ret.position.z = component["position"]["z"];

        ret.rotation.x = component["rotation"]["x"];
        ret.rotation.y = component["rotation"]["y"];
        ret.rotation.z = component["rotation"]["z"];

        ret.scale.x = component["scale"]["x"];
        ret.scale.y = component["scale"]["y"];
        ret.scale.z = component["scale"]["z"];

        return ret;
    }

    ColorRGBA convertColor(const nlohmann::json &j) {
        return {j.at("r"), j.at("g"), j.at("b"), j["a"]};
    }

    CameraComponent *getCameraComponent(const nlohmann::json &component) {
        auto *ret = new CameraComponent();
        ret->camera.type = convertCameraType(component["cameraType"]);
        if (ret->camera.type == PERSPECTIVE) {
            ret->camera.nearClip = component["nearClip"];
            ret->camera.farClip = component["farClip"];
            ret->camera.fov = component["fov"];
            ret->camera.aspectRatio = component["aspectRatio"];
        } else {
            ret->camera.nearClip = component["nearClip"];
            ret->camera.farClip = component["farClip"];
            ret->camera.left = component["left"];
            ret->camera.right = component["right"];
            ret->camera.top = component["top"];
            ret->camera.bottom = component["bottom"];
        }
        return ret;
    }

    TransformComponent *getTransformComponent(const nlohmann::json &component) {
        auto *ret = new TransformComponent();
        ret->transform = convertTransform(component);
        return ret;
    }

    MeshRenderComponent *getMeshRenderComponent(const nlohmann::json &component) {
        auto *ret = new MeshRenderComponent();
        ret->castShadows = component.value("castShadows", false);
        ret->receiveShadows = component.value("receiveShadows", false);
        ret->mesh = {component["mesh"]["bundle"], component["mesh"]["asset"]};
        ret->material = {component["material"]["bundle"], component["material"]["asset"]};
        return ret;
    }

    SkyboxComponent *getSkyboxComponent(const nlohmann::json &component) {
        auto *ret = new SkyboxComponent();
        int i = 0;
        for (auto &node : component["paths"]) {
            ret->paths.at(i++) = {node["bundle"], node["asset"]};
        }
        return ret;
    }

    LightComponent *getLightComponent(const nlohmann::json &component) {
        auto *ret = new LightComponent();
        ret->light.type = convertLightType(component["lightType"]);

        ret->light.ambient.x = component["ambient"]["r"];
        ret->light.ambient.y = component["ambient"]["g"];
        ret->light.ambient.z = component["ambient"]["b"];
        ret->light.diffuse.x = component["diffuse"]["r"];
        ret->light.diffuse.y = component["diffuse"]["g"];
        ret->light.diffuse.z = component["diffuse"]["b"];
        ret->light.specular.x = component["specular"]["r"];
        ret->light.specular.y = component["specular"]["g"];
        ret->light.specular.z = component["specular"]["b"];

        switch (ret->light.type) {
            case LIGHT_POINT:
                ret->light.constant = component["constant"];
                ret->light.linear = component["linear"];
                ret->light.quadratic = component["quadratic"];
                break;
            case LIGHT_SPOT:
                ret->light.direction.x = component["direction"]["x"];
                ret->light.direction.y = component["direction"]["y"];
                ret->light.direction.z = component["direction"]["z"];
                ret->light.cutOff = component["cutOff"];
                ret->light.outerCutOff = component["outerCutOff"];
                ret->light.constant = component["constant"];
                ret->light.linear = component["linear"];
                ret->light.quadratic = component["quadratic"];
                break;
            case LIGHT_DIRECTIONAL:
                ret->light.direction.x = component["direction"]["x"];
                ret->light.direction.y = component["direction"]["y"];
                ret->light.direction.z = component["direction"]["z"];
                break;
        }
        return ret;
    }

    ScriptComponent *getScriptComponent(const nlohmann::json &component) {
        auto *ret = new ScriptComponent();
        ret->runtime = component["runtime"];
        ret->assembly = component["assembly"];
        ret->nameSpace = component["nameSpace"];
        ret->className = component["className"];
        ret->queue = component["queue"];
        return ret;
    }

    Component *getComponent(const nlohmann::json &component) {
        Component *ret;
        switch (convertComponentType(component["componentType"])) {
            case TRANSFORM:
                ret = getTransformComponent(component);
                break;
            case CAMERA:
                ret = getCameraComponent(component);
                break;
            case LIGHT:
                ret = getLightComponent(component);
                break;
            case SCRIPT:
                ret = getScriptComponent(component);
                break;
            case MESH_RENDER:
                ret = getMeshRenderComponent(component);
                break;
            case SKYBOX:
                ret = getSkyboxComponent(component);
                break;
            default:
                throw std::runtime_error("Unrecognized component type");
        }
        ret->enabled = component["enabled"];
        return ret;
    }

    ComponentDeserializer::ComponentDeserializer() = default;

    Component *ComponentDeserializer::deserialize(std::istream &stream) {
        nlohmann::json j;
        stream >> j;
        return getComponent(j);
    }
}