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

#include "jsoncommon.hpp"

#include "../../../extern/json.hpp"

using namespace mana;

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

std::string convertComponentType(ComponentType type) {
    switch (type) {
        case TRANSFORM:
            return "transform";
        case CAMERA:
            return "camera";
        case LIGHT:
            return "light";
        case SCRIPT:
            return "script";
        case MESH_RENDER:
            return "mesh_render";
        case SKYBOX:
            return "skybox";
        default:
            throw std::runtime_error("");
    }
}

CameraType convertCameraType(const std::string &str) {
    if (str == "perspective")
        return PERSPECTIVE;
    else if (str == "orthographic")
        return ORTHOGRAPHIC;
    throw std::runtime_error("Invalid camera type " + str);
}

std::string convertCameraType(CameraType type) {
    switch (type) {
        case mana::PERSPECTIVE:
            return "perspective";
        case mana::ORTHOGRAPHIC:
            return "orthographic";
        default:
            throw std::runtime_error("");
    }
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

std::string convertLightType(LightType type) {
    switch (type) {
        case mana::LIGHT_DIRECTIONAL:
            return "directional";
        case mana::LIGHT_POINT:
            return "point";
        case mana::LIGHT_SPOT:
            return "spot";
        default:
            throw std::runtime_error("");
    }
}

Vec3f convertVector3(const nlohmann::json &v) {
    Vec3f ret;
    ret.x = v.value("x", v.value("r", 0.0f));
    ret.y = v.value("y", v.value("g", 0.0f));
    ret.z = v.value("z", v.value("b", 0.0f));
    return ret;
}

nlohmann::json convertVector3(const Vec3f &v) {
    nlohmann::json ret;
    ret["x"] = v.x;
    ret["y"] = v.y;
    ret["z"] = v.z;
    return ret;
}

Vec4f convertVector4(const nlohmann::json &v) {
    Vec4f ret;
    ret.x = v.value("x", v.value("r", 0.0f));
    ret.y = v.value("y", v.value("g", 0.0f));
    ret.z = v.value("z", v.value("b", 0.0f));
    ret.w = v.value("w", v.value("a", 0.0f));
    return ret;
}

nlohmann::json convertVector4(const Vec4f &v) {
    nlohmann::json ret;
    ret["x"] = v.x;
    ret["y"] = v.y;
    ret["z"] = v.z;
    ret["w"] = v.w;
    return ret;
}

Transform convertTransform(const nlohmann::json &component) {
    Transform ret;
    ret.position = convertVector3(component["position"]);
    ret.rotation = convertVector3(component["rotation"]);
    ret.scale = convertVector3(component["scale"]);
    return ret;
}

nlohmann::json convertTransform(const Transform &transform) {
    nlohmann::json ret;
    ret["position"] = convertVector3(transform.position);
    ret["rotation"] = convertVector3(transform.rotation);
    ret["scale"] = convertVector3(transform.scale);
    return ret;
}

AssetPath convertAssetPath(const nlohmann::json &path) {
    AssetPath ret;
    ret.bundle = path["bundle"];
    ret.asset = path["asset"];
    return ret;
}

nlohmann::json convertAssetPath(AssetPath path) {
    nlohmann::json ret;
    ret["bundle"] = path.bundle;
    ret["asset"] = path.asset;
    return ret;
}

CameraComponent *convertCameraComponent(const nlohmann::json &component) {
    auto *ret = new CameraComponent();
    ret->camera.type = convertCameraType(static_cast<std::string>(component["cameraType"]));
    ret->camera.nearClip = component["nearClip"];
    ret->camera.farClip = component["farClip"];
    if (ret->camera.type == PERSPECTIVE) {
        ret->camera.fov = component["fov"];
        ret->camera.aspectRatio = component["aspectRatio"];
    } else {
        ret->camera.left = component["left"];
        ret->camera.right = component["right"];
        ret->camera.top = component["top"];
        ret->camera.bottom = component["bottom"];
    }
    return ret;
}

nlohmann::json convertCameraComponent(const CameraComponent &component) {
    nlohmann::json ret;
    ret["cameraType"] = convertCameraType(component.camera.type);
    ret["nearClip"] = component.camera.nearClip;
    ret["farClip"] = component.camera.farClip;
    if (component.camera.type == PERSPECTIVE) {
        ret["fov"] = component.camera.fov;
        ret["aspectRatio"] = component.camera.aspectRatio;
    } else {
        ret["left"] = component.camera.left;
        ret["right"] = component.camera.right;
        ret["top"] = component.camera.top;
        ret["bottom"] = component.camera.bottom;
    }
    return ret;
}

TransformComponent *convertTransformComponent(const nlohmann::json &component) {
    auto *ret = new TransformComponent();
    ret->transform = convertTransform(component);
    ret->parent = component["parent"];
    return ret;
}

nlohmann::json convertTransformComponent(const TransformComponent &component) {
    nlohmann::json ret;
    ret["transform"] = convertTransform(component.transform);
    ret["parent"] = component.parent;
    return ret;
}

MeshRenderComponent *convertMeshRenderComponent(const nlohmann::json &component) {
    auto *ret = new MeshRenderComponent();
    ret->castShadows = component.value("castShadows", false);
    ret->receiveShadows = component.value("receiveShadows", false);
    ret->mesh = convertAssetPath(component["mesh"]);
    ret->material = convertAssetPath(component["material"]);
    return ret;
}

nlohmann::json convertMeshRenderComponent(const MeshRenderComponent &component) {
    nlohmann::json ret;
    ret["castShadows"] = component.castShadows;
    ret["receiveShadows"] = component.receiveShadows;
    ret["mesh"] = convertAssetPath(component.mesh);
    ret["material"] = convertAssetPath(component.material);
    return ret;
}

SkyboxComponent *convertSkyboxComponent(const nlohmann::json &component) {
    auto *ret = new SkyboxComponent();
    int i = 0;
    for (auto &node : component["paths"]) {
        ret->paths.at(i++) = convertAssetPath(node);
    }
    return ret;
}

nlohmann::json convertSkyboxComponent(const SkyboxComponent &component) {
    nlohmann::json ret;
    for (const auto &path : component.paths) {
        ret["paths"] += convertAssetPath(path);
    }
    return ret;
}

LightComponent *convertLightComponent(const nlohmann::json &component) {
    auto *ret = new LightComponent();

    ret->light.type = convertLightType(static_cast<std::string>(component["lightType"]));

    ret->light.ambient = convertVector3(component["ambient"]);
    ret->light.diffuse = convertVector3(component["diffuse"]);
    ret->light.specular = convertVector3(component["specular"]);

    switch (ret->light.type) {
        case LIGHT_POINT:
            ret->light.constant = component["constant"];
            ret->light.linear = component["linear"];
            ret->light.quadratic = component["quadratic"];
            break;
        case LIGHT_SPOT:
            ret->light.direction = convertVector3(component["direction"]);
            ret->light.cutOff = component["cutOff"];
            ret->light.outerCutOff = component["outerCutOff"];
            ret->light.constant = component["constant"];
            ret->light.linear = component["linear"];
            ret->light.quadratic = component["quadratic"];
            break;
        case LIGHT_DIRECTIONAL:
            ret->light.direction = convertVector3(component["direction"]);
            break;
    }
    return ret;
}

nlohmann::json convertLightComponent(const LightComponent &component) {
    nlohmann::json ret;
    ret["lightType"] = convertLightType(component.light.type);
    ret["ambient"] = convertVector3(component.light.ambient);
    ret["diffuse"] = convertVector3(component.light.diffuse);
    ret["specular"] = convertVector3(component.light.specular);

    switch (component.light.type) {
        case mana::LIGHT_POINT:
            ret["constant"] = component.light.constant;
            ret["linear"] = component.light.linear;
            ret["quadratic"] = component.light.quadratic;
            break;
        case mana::LIGHT_SPOT:
            ret["direction"] = convertVector3(component.light.direction);
            ret["cutOff"] = component.light.cutOff;
            ret["outerCutOff"] = component.light.outerCutOff;
            ret["constant"] = component.light.constant;
            ret["linear"] = component.light.linear;
            ret["quadratic"] = component.light.quadratic;
            break;
        case mana::LIGHT_DIRECTIONAL:
            ret["direction"] = convertVector3(component.light.direction);
            break;
        default:
            throw std::runtime_error("");
    }

    return ret;
}

ScriptComponent *convertScriptComponent(const nlohmann::json &component) {
    auto *ret = new ScriptComponent();
    ret->runtime = component["runtime"];
    ret->assembly = component["assembly"];
    ret->nameSpace = component["nameSpace"];
    ret->className = component["className"];
    ret->queue = component["queue"];
    return ret;
}

nlohmann::json convertScriptComponent(const ScriptComponent &component) {
    nlohmann::json ret;
    ret["runtime"] = component.runtime;
    ret["assembly"] = component.assembly;
    ret["nameSpace"] = component.nameSpace;
    ret["className"] = component.className;
    ret["queue"] = component.queue;
    return ret;
}

Component *convertComponent(const nlohmann::json &component) {
    Component *ret = nullptr;
    switch (convertComponentType(static_cast<std::string>(component["componentType"]))) {
        case TRANSFORM:
            ret = convertTransformComponent(component);
            break;
        case CAMERA:
            ret = convertCameraComponent(component);
            break;
        case LIGHT:
            ret = convertLightComponent(component);
            break;
        case SCRIPT:
            ret = convertScriptComponent(component);
            break;
        case MESH_RENDER:
            ret = convertMeshRenderComponent(component);
            break;
        case SKYBOX:
            ret = convertSkyboxComponent(component);
            break;
        default:
            throw std::runtime_error("Unrecognized component type");
    }
    ret->enabled = component["enabled"];
    return ret;
}

nlohmann::json convertComponent(const Component &component) {
    nlohmann::json ret;
    ret["enabled"] = component.enabled;
    switch (component.componentType) {
        case TRANSFORM:
            ret = convertTransformComponent(dynamic_cast<const TransformComponent &>(component));
            break;
        case CAMERA:
            ret = convertCameraComponent(dynamic_cast<const CameraComponent &>(component));
            break;
        case LIGHT:
            ret = convertLightComponent(dynamic_cast<const LightComponent &>(component));
            break;
        case SCRIPT:
            ret = convertScriptComponent(dynamic_cast<const ScriptComponent &>(component));
            break;
        case MESH_RENDER:
            ret = convertMeshRenderComponent(dynamic_cast<const MeshRenderComponent &>(component));
            break;
        case SKYBOX:
            ret = convertSkyboxComponent(dynamic_cast<const SkyboxComponent &>(component));
            break;
        default:
            throw std::runtime_error("Unrecognized component type");
    }
    return ret;
}

Node convertNode(const nlohmann::json &node) {
    Node ret;
    ret.enabled = node["enabled"];
    for (auto &component : node["components"]) {
        ret.addComponentPointer(convertComponent(component));
    }
    return ret;
}

nlohmann::json convertNode(const Node &node) {
    nlohmann::json ret;
    ret["enabled"] = node.enabled;
    for (auto &component : node.components) {
        ret["components"] += convertComponent(*component.second);
    }
    return ret;
}

Scene convertScene(const nlohmann::json &scene) {
    Scene ret;
    ret.name = scene["name"];
    for (auto &node : scene["nodes"]) {
        std::string name = node["name"];
        ret.nodes[name] = convertNode(node);
    }
    return ret;
}

nlohmann::json convertScene(const Scene &scene) {
    nlohmann::json ret;
    ret["name"] = scene.name;
    for (auto &pair : scene.nodes) {
        auto node = convertNode(pair.second);
        node["name"] = pair.first;
        ret["nodes"] += node;
    }
    return ret;
}
