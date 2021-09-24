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

#include "engine/io/schema/ecsschema.hpp"
#include "engine/io/schema/mathschema.hpp"

namespace engine {
    ComponentType &operator<<(ComponentType &value, const Message &message) {
        auto str = message.getString();
        if (str == "transform")
            value = TRANSFORM;
        else if (str == "camera")
            value = CAMERA;
        else if (str == "light")
            value = LIGHT;
        else if (str == "script_mono")
            value = SCRIPT_MONO;
        else if (str == "sync_mono")
            value = SYNC_MONO;
        else if (str == "mesh_render")
            value = MESH_RENDER;
        else if (str == "skybox")
            value = SKYBOX;
        else
            throw std::runtime_error("Invalid component type " + str);
        return value;
    }

    Message &operator<<(Message &message, ComponentType type) {
        switch (type) {
            case TRANSFORM:
                message = "transform";
                break;
            case CAMERA:
                message = "camera";
                break;
            case LIGHT:
                message = "light";
                break;
            case SCRIPT_MONO:
                message = "script_mono";
                break;
            case SYNC_MONO:
                message = "sync_mono";
                break;
            case MESH_RENDER:
                message = "mesh_render";
                break;
            case SKYBOX:
                message = "skybox";
                break;
            default:
                throw std::runtime_error("");
        }
        return message;
    }

    CameraType &operator<<(CameraType &value, const Message &message) {
        auto str = message.getString();
        if (str == "perspective")
            value = PERSPECTIVE;
        else if (str == "orthographic")
            value = ORTHOGRAPHIC;
        else
            throw std::runtime_error("Invalid camera type " + str);
        return value;
    }

    Message &operator<<(Message &message, CameraType type) {
        switch (type) {
            case PERSPECTIVE:
                message = "perspective";
                break;
            case ORTHOGRAPHIC:
                message = "orthographic";
                break;
            default:
                throw std::runtime_error("");
        }
        return message;
    }

    LightType &operator<<(LightType &value, const Message &message) {
        auto str = message.getString();
        if (str == "directional")
            value = LIGHT_DIRECTIONAL;
        else if (str == "point")
            value = LIGHT_POINT;
        else if (str == "spot")
            value = LIGHT_SPOT;
        else
            throw std::runtime_error("Invalid light type " + str);
        return value;
    }

    Message &operator<<(Message &message, LightType type) {
        switch (type) {
            case LIGHT_DIRECTIONAL:
                message = "directional";
                break;
            case LIGHT_POINT:
                message = "point";
                break;
            case LIGHT_SPOT:
                message = "spot";
                break;
            default:
                throw std::runtime_error("");
        }
        return message;
    }

    AssetPath &operator<<(AssetPath &value, const Message &message) {
        auto map = message.getMap();
        value.bundle = message["bundle"].getString();
        value.asset = map["asset"].getString();
        return value;
    }

    Message &operator<<(Message &message, const AssetPath &path) {
        auto map = std::map<std::string, Message>();
        map["bundle"] = path.bundle;
        map["asset"] = path.asset;
        message = map;
        return message;
    }

    CameraComponent *&operator<<(CameraComponent *&component, const Message &message) {
        component = new CameraComponent();
        component->camera.type << message["cameraType"];
        component->camera.nearClip = message["nearClip"];
        component->camera.farClip = message["farClip"];
        if (component->camera.type == PERSPECTIVE) {
            component->camera.fov = message["fov"];
            component->camera.aspectRatio = message["aspectRatio"];
        } else {
            component->camera.left = message["left"];
            component->camera.right = message["right"];
            component->camera.top = message["top"];
            component->camera.bottom = message["bottom"];
        }
        return component;
    }

    Message &operator<<(Message &message, const CameraComponent *&component) {
        message = std::map<std::string, Message>();
        message["cameraType"] << component->camera.type;
        message["nearClip"] = component->camera.nearClip;
        message["farClip"] = component->camera.farClip;
        if (component->camera.type == PERSPECTIVE) {
            message["fov"] = component->camera.fov;
            message["aspectRatio"] = component->camera.aspectRatio;
        } else {
            message["left"] = component->camera.left;
            message["right"] = component->camera.right;
            message["top"] = component->camera.top;
            message["bottom"] = component->camera.bottom;
        }
        return message;
    }

    TransformComponent *&operator<<(TransformComponent *&component, const Message &message) {
        component = new TransformComponent();
        component->transform << message["transform"];
        component->parent = message["parent"].get<std::string>();
        return component;
    }

    Message &operator<<(Message &message, const TransformComponent *&component) {
        message = std::map<std::string, Message>();
        message["transform"] << component->transform;
        message["parent"] = component->parent;
        return message;
    }

    MeshRenderComponent *&operator<<(MeshRenderComponent *&component, const Message &message) {
        component = new MeshRenderComponent();
        component->castShadows = message["castShadows"];
        component->receiveShadows = message["receiveShadows"];
        component->mesh << message["mesh"];
        component->material << message["material"];
        return component;
    }

    Message &operator<<(Message &message, const MeshRenderComponent *&component) {
        message = std::map<std::string, Message>();
        message["castShadows"] = component->castShadows;
        message["receiveShadows"] = component->receiveShadows;
        message["mesh"] << component->mesh;
        message["material"] << component->material;
        return message;
    }

    SkyboxComponent *&operator<<(SkyboxComponent *&component, const Message &message) {
        component = new SkyboxComponent();
        int i = 0;
        for (auto &node : message["paths"].getVector()) {
            component->paths.at(i++) << node;
        }
        return component;
    }

    Message &operator<<(Message &message, const SkyboxComponent *&component) {
        message = std::map<std::string, Message>();
        auto vector = std::vector<Message>();
        for (const auto &path : component->paths) {
            Message m;
            m << path;
            vector.emplace_back(m);
        }
        message["paths"] = vector;
        return message;
    }

    LightComponent *&operator<<(LightComponent *&component, const Message &message) {
        component = new LightComponent();

        component->light.type << message["lightType"];

        component->light.ambient << message["ambient"];
        component->light.diffuse << message["diffuse"];
        component->light.specular << message["specular"];

        switch (component->light.type) {
            case LIGHT_POINT:
                component->light.constant = message["constant"];
                component->light.linear = message["linear"];
                component->light.quadratic = message["quadratic"];
                break;
            case LIGHT_SPOT:
                component->light.direction << message["direction"];
                component->light.cutOff = message["cutOff"];
                component->light.outerCutOff = message["outerCutOff"];
                component->light.constant = message["constant"];
                component->light.linear = message["linear"];
                component->light.quadratic = message["quadratic"];
                break;
            case LIGHT_DIRECTIONAL:
                component->light.direction << message["direction"];
                break;
        }
        return component;
    }

    Message &operator<<(Message &message, const LightComponent *&component) {
        message = std::map<std::string, Message>();
        message["lightType"] << component->light.type;
        message["ambient"] << component->light.ambient;
        message["diffuse"] << component->light.diffuse;
        message["specular"] << component->light.specular;

        switch (component->light.type) {
            case engine::LIGHT_POINT:
                message["constant"] = component->light.constant;
                message["linear"] = component->light.linear;
                message["quadratic"] = component->light.quadratic;
                break;
            case engine::LIGHT_SPOT:
                message["direction"] << component->light.direction;
                message["cutOff"] = component->light.cutOff;
                message["outerCutOff"] = component->light.outerCutOff;
                message["constant"] = component->light.constant;
                message["linear"] = component->light.linear;
                message["quadratic"] = component->light.quadratic;
                break;
            case engine::LIGHT_DIRECTIONAL:
                message["direction"] << component->light.direction;
                break;
            default:
                throw std::runtime_error("");
        }
        return message;
    }

    MonoScriptComponent *&operator<<(MonoScriptComponent *&component, const Message &message) {
        component = new MonoScriptComponent();
        component->assembly = message["assembly"].getString();
        component->nameSpace = message["nameSpace"].getString();
        component->className = message["className"].getString();
        component->queue = message["queue"];
        return component;
    }

    Message &operator<<(Message &message, const MonoScriptComponent *&component) {
        message = std::map<std::string, Message>();
        message["assembly"] = component->assembly;
        message["nameSpace"] = component->nameSpace;
        message["className"] = component->className;
        message["queue"] = component->queue;
        return message;
    }

    MonoSyncComponent *&operator<<(MonoSyncComponent *&component, const Message &message) {
        component = new MonoSyncComponent();
        return component;
    }

    Message &operator<<(Message &message, const MonoSyncComponent *&component) {
        message = std::map<std::string, Message>();
        return message;
    }

    Component *&operator<<(Component *&value, const Message &message) {
        auto map = message.getMap();

        ComponentType type;
        type << map.at("componentType");

        Component *ret;
        switch (type) {
            case TRANSFORM:
                TransformComponent *transform;
                transform << message;
                ret = transform;
                break;
            case CAMERA:
                CameraComponent *camera;
                camera << message;
                ret = camera;
                break;
            case LIGHT:
                LightComponent *light;
                light << message;
                ret = light;
                break;
            case SCRIPT_MONO:
                MonoScriptComponent *script;
                script << message;
                ret = script;
                break;
            case SYNC_MONO:
                MonoSyncComponent *sync;
                sync << message;
                ret = sync;
                break;
            case MESH_RENDER:
                MeshRenderComponent *render;
                render << message;
                ret = render;
                break;
            case SKYBOX:
                SkyboxComponent *skybox;
                skybox << message;
                ret = skybox;
                break;
            default:
                throw std::runtime_error("Unrecognized component type");
        }
        ret->enabled = map.at("enabled");
        value = ret;
        return value;
    }

    Message &operator<<(Message &message, const Component *&component) {
        const TransformComponent *transform;
        const CameraComponent *camera;
        const LightComponent *light;
        const MonoScriptComponent *script;
        const MonoSyncComponent *sync;
        const MeshRenderComponent *render;
        const SkyboxComponent *skybox;
        switch (component->componentType) {
            case TRANSFORM:
                transform = dynamic_cast<const TransformComponent *>(component);
                message << transform;
                break;
            case CAMERA:
                camera = dynamic_cast<const CameraComponent *>(component);
                message << camera;
                break;
            case LIGHT:
                light = dynamic_cast<const LightComponent *>(component);
                message << light;
                break;
            case SCRIPT_MONO:
                script = dynamic_cast<const MonoScriptComponent *>(component);
                message << script;
                break;
            case SYNC_MONO:
                sync = dynamic_cast<const MonoSyncComponent*>(component);
                message << sync;
                break;
            case MESH_RENDER:
                render = dynamic_cast<const MeshRenderComponent *>(component);
                message << render;
                break;
            case SKYBOX:
                skybox = dynamic_cast<const SkyboxComponent *>(component);
                message << skybox;
                break;
            default:
                throw std::runtime_error("Unrecognized component type");
        }
        auto map = message.getMap();
        map["enabled"] = component->enabled;
        map["componentType"] << component->componentType;
        message = map;
        return message;
    }

    Node &operator<<(Node &node, const Message &message) {
        auto map = message.getMap();
        node.enabled = map.at("enabled");
        for (auto &component : map.at("components").getVector()) {
            Component *comp;
            comp << component;
            node.addComponentPointer(comp);
        }
        return node;
    }

    Message &operator<<(Message &message, const Node &node) {
        message = std::map<std::string, Message>();
        message["enabled"] = node.enabled;
        auto vector = std::vector<Message>();
        for (auto &component : node.components) {
            const Component *ptr = component.second;
            Message comp;
            comp << ptr;
            vector.emplace_back(comp);
        }
        message["components"] = vector;
        return message;
    }

    Scene &operator<<(Scene &scene, const Message &message) {
        auto map = message.getMap();
        scene.name = map.at("name").getString();
        for (auto &node : map.at("nodes").getMap()) {
            Node n;
            n << node.second;
            scene.nodes[node.first] = std::move(n);
        }
        return scene;
    }

    Message &operator<<(Message &message, const Scene &scene) {
        message = std::map<std::string, Message>();
        message["name"] = scene.name;

        auto nodes = std::map<std::string, Message>();
        for (auto &node : scene.nodes) {
            Message n;
            n << node.second;
            nodes[node.first] = n;
        }

        message["nodes"] = nodes;
        return message;
    }
}