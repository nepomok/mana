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

#include "engine/io/sceneloader.hpp"
#include "engine/ecs/components/cameracomponent.hpp"
#include "engine/ecs/components/lightcomponent.hpp"
#include "engine/ecs/components/scriptcomponent.hpp"
#include "engine/ecs/components/transformcomponent.hpp"
#include "engine/ecs/components/rendercomponent.hpp"

#include "engine/io/imageloader.hpp"
#include "engine/io/meshloader.hpp"
#include "engine/io/file.hpp"

#include "engine/render/renderer3d.hpp"

#include "extern/json.hpp"

namespace mana {
    Component::ComponentType convertComponent(const std::string &str) {
        if (str == "transform")
            return Component::TRANSFORM;
        else if (str == "render")
            return Component::RENDER;
        else if (str == "camera")
            return Component::CAMERA;
        else if (str == "light")
            return Component::LIGHT;
        else if (str == "script")
            return Component::SCRIPT;
        throw std::runtime_error("Unrecognized component type " + str);
    }

    CameraType convertCamera(const std::string &str) {
        if (str == "perspective")
            return PERSPECTIVE;
        else if (str == "orthographic")
            return ORTHOGRAPHIC;
        throw std::runtime_error("Unrecognized camera type " + str);
    }

    LightType convertLight(const std::string &str) {
        if (str == "directional")
            return LIGHT_DIRECTIONAL;
        else if (str == "point")
            return LIGHT_POINT;
        else if (str == "spot")
            return LIGHT_SPOT;
        throw std::runtime_error("Unrecognized light type " + str);
    }

    CameraComponent getCamera(const nlohmann::json &component) {
        CameraComponent ret;
        ret.cameraType = convertCamera(component.value("cameraType", "perspective"));
        if (ret.cameraType == PERSPECTIVE) {
            ret.nearClip = component.value("nearClip", 1.0);
            ret.farClip = component.value("farClip", 1.0);
            ret.fov = component.value("fov", 1.0);
            ret.aspectRatio = component.value("aspectRatio", 1.0);
        } else {
            ret.nearClip = component.value("nearClip", 1.0);
            ret.farClip = component.value("farClip", 1.0);
            ret.left = component.value("left", 1.0);
            ret.right = component.value("right", 1.0);
            ret.top = component.value("top", 1.0);
            ret.bottom = component.value("bottom", 1.0);
        }
        return ret;
    }

    TransformComponent getTransform(const nlohmann::json &component) {
        TransformComponent ret;
        if (component.contains("position")) {
            ret.transform.position.x = component["position"].value("x", 0.0);
            ret.transform.position.y = component["position"].value("y", 0.0);
            ret.transform.position.z = component["position"].value("z", 0.0);
        }
        if (component.contains("rotation")) {
            ret.transform.rotation.x = component["rotation"].value("x", 0.0);
            ret.transform.rotation.y = component["rotation"].value("y", 0.0);
            ret.transform.rotation.z = component["rotation"].value("z", 0.0);
        }
        if (component.contains("scale")) {
            ret.transform.scale.x = component["scale"].value("x", 1.0);
            ret.transform.scale.y = component["scale"].value("y", 1.0);
            ret.transform.scale.z = component["scale"].value("z", 1.0);
        }
        return ret;
    }

    RenderComponent getRenderComponent(const nlohmann::json &component, RenderAllocator &alloc) {
        RenderComponent ret;
        auto vs = Renderer3D::preprocessHlsl(File::readAllText(component["vertexShaderPath"]));
        auto fs = Renderer3D::preprocessHlsl(File::readAllText(component["fragmentShaderPath"]));
        ret.shader = alloc.allocateShaderProgram(vs, fs);

        for (const auto &entry : component["textureMapping"]) {
            ret.shader->setTexture(entry["name"], entry["slot"]);
        }

        for (auto &meshPath : component["meshPaths"]) {
            auto mesh = MeshLoader::load(meshPath);
            ret.meshData.emplace_back(alloc.allocateMesh(mesh));
        }

        for (const auto &texturePath : component["texturePaths"]) {
            auto img = ImageLoader::load(texturePath);
            auto attr = RenderTexture::Attributes();
            attr.size = img.getSize();
            auto tex = alloc.allocateTexture(attr);
            tex->upload(img);
            ret.textures.emplace_back(tex);
        }
        return ret;
    }

    LightComponent getLight(const nlohmann::json &component) {
        LightComponent ret;
        ret.lightType = convertLight(component.value("lightType", "directional"));
        if (component.contains("ambient")) {
            ret.ambient.x = component["ambient"].value("r", 1.0);
            ret.ambient.y = component["ambient"].value("g", 1.0);
            ret.ambient.z = component["ambient"].value("b", 1.0);
        }
        if (component.contains("diffuse")) {
            ret.diffuse.x = component["diffuse"].value("r", 1.0);
            ret.diffuse.y = component["diffuse"].value("g", 1.0);
            ret.diffuse.z = component["diffuse"].value("b", 1.0);
        }
        if (component.contains("specular")) {
            ret.specular.x = component["specular"].value("r", 1.0);
            ret.specular.y = component["specular"].value("g", 1.0);
            ret.specular.z = component["specular"].value("b", 1.0);
        }
        switch (ret.lightType) {
            case LIGHT_POINT:
                ret.constant = component.value("constant", 1.0);
                ret.linear = component.value("linear", 1.0);
                ret.quadratic = component.value("quadratic", 1.0);
                break;
            case LIGHT_DIRECTIONAL:
            case LIGHT_SPOT:
                if (component.contains("direction")) {
                    ret.direction.x = component["direction"].value("x", 1.0);
                    ret.direction.y = component["direction"].value("y", 1.0);
                    ret.direction.z = component["direction"].value("z", 1.0);
                }
                ret.cutOff = component.value("cutOff", 1.0);
                ret.outerCutOff = component.value("outerCutOff", 1.0);
                ret.constant = component.value("constant", 1.0);
                ret.linear = component.value("linear", 1.0);
                ret.quadratic = component.value("quadratic", 1.0);
                break;
        }
        return ret;
    }

    Scene SceneLoader::loadJson(std::string jsonStr, RenderAllocator &allocator) {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        Scene ret;
        for (auto &node : j["nodes"]) {
            std::string nodeName = node["nodeName"];
            if (ret.nodes.find(nodeName) != ret.nodes.end())
                throw std::runtime_error("Node with name " + nodeName + " already exists.");
            Node n;
            for (auto &component : node["components"]) {
                switch (convertComponent(component["componentType"])) {
                    case Component::TRANSFORM:
                        n.addComponent(getTransform(component));
                        break;
                    case Component::CAMERA:
                        n.addComponent(getCamera(component));
                        break;
                    case Component::RENDER:
                        n.addComponent(getRenderComponent(component, allocator));
                        break;
                    case Component::LIGHT:
                        n.addComponent(getLight(component));
                        break;
                    case Component::SCRIPT:
                        throw std::runtime_error("Not Implemented");
                    default:
                        throw std::runtime_error("Unrecognized component type");
                }
            }
            ret.nodes[nodeName] = n;
        }
        return ret;
    }
}