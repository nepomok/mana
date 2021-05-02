#include <string>

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

#include "engine/io/scenefile.hpp"
#include "engine/ecs/components.hpp"
#include "engine/render/renderer3d.hpp"
#include "engine/io/assetfile.hpp"
#include "engine/io/imagefile.hpp"
#include "engine/script/mono/monoscript.hpp"

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
        ret.cameraType = convertCamera(component["cameraType"]);
        if (ret.cameraType == PERSPECTIVE) {
            ret.nearClip = component["nearClip"];
            ret.farClip = component["farClip"];
            ret.fov = component["fov"];
            ret.aspectRatio = component["aspectRatio"];
        } else {
            ret.nearClip = component["nearClip"];
            ret.farClip = component["farClip"];
            ret.left = component["left"];
            ret.right = component["right"];
            ret.top = component["top"];
            ret.bottom = component["bottom"];
        }
        return ret;
    }

    TransformComponent getTransform(const nlohmann::json &component) {
        TransformComponent ret;
        ret.transform.position.x = component["position"]["x"];
        ret.transform.position.y = component["position"]["y"];
        ret.transform.position.z = component["position"]["z"];

        ret.transform.rotation.x = component["rotation"]["x"];
        ret.transform.rotation.y = component["rotation"]["y"];
        ret.transform.rotation.z = component["rotation"]["z"];

        ret.transform.scale.x = component["scale"]["x"];
        ret.transform.scale.y = component["scale"]["y"];
        ret.transform.scale.z = component["scale"]["z"];

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
            auto meshAsset = AssetFile(meshPath);
            ret.meshData.emplace_back(alloc.allocateMesh(meshAsset.getMesh()));
        }

        for (const auto &texturePath : component["texturePaths"]) {
            auto img = ImageFile(texturePath);
            auto attr = RenderTexture::Attributes();
            attr.size = img.getBuffer().getSize();
            auto tex = alloc.allocateTexture(attr);
            tex->upload(img.getBuffer());
            ret.textures.emplace_back(tex);
        }
        return ret;
    }

    LightComponent getLight(const nlohmann::json &component) {
        LightComponent ret;
        ret.lightType = convertLight(component["lightType"]);

        ret.ambient.x = component["ambient"]["r"];
        ret.ambient.y = component["ambient"]["g"];
        ret.ambient.z = component["ambient"]["b"];
        ret.diffuse.x = component["diffuse"]["r"];
        ret.diffuse.y = component["diffuse"]["g"];
        ret.diffuse.z = component["diffuse"]["b"];
        ret.specular.x = component["specular"]["r"];
        ret.specular.y = component["specular"]["g"];
        ret.specular.z = component["specular"]["b"];

        switch (ret.lightType) {
            case LIGHT_POINT:
                ret.constant = component["constant"];
                ret.linear = component["linear"];
                ret.quadratic = component["quadratic"];
                break;
            case LIGHT_SPOT:
                ret.direction.x = component["direction"]["x"];
                ret.direction.y = component["direction"]["y"];
                ret.direction.z = component["direction"]["z"];
                ret.cutOff = component["cutOff"];
                ret.outerCutOff = component["outerCutOff"];
                ret.constant = component["constant"];
                ret.linear = component["linear"];
                ret.quadratic = component["quadratic"];
                break;
            case LIGHT_DIRECTIONAL:
                ret.direction.x = component["direction"]["x"];
                ret.direction.y = component["direction"]["y"];
                ret.direction.z = component["direction"]["z"];
                break;
        }
        return ret;
    }

    ScriptComponent getScript(const nlohmann::json &component, MonoCppRuntime &monoRuntime) {
        ScriptComponent ret;
        ret.script = new MonoScript(monoRuntime.loadAssembly(component["assemblyFilePath"]),
                                    component["scriptNamespace"],
                                    component["scriptClass"]);
        return ret;
    }

    Scene parseJsonScene(const std::string &jsonStr, RenderAllocator &allocator, MonoCppRuntime &monoRuntime) {
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
                        n.addComponent(getScript(component, monoRuntime));
                        break;
                    default:
                        throw std::runtime_error("Unrecognized component type");
                }
            }
            ret.nodes[nodeName] = n;
        }
        return ret;
    }

    SceneFile::SceneFile() = default;

    SceneFile::SceneFile(const std::string &filepath) {
        fileText = File::readAllText(filepath);
    }

    void SceneFile::open(const std::string &filePath) {
        fileText = File::readAllText(filePath);
    }

    void SceneFile::close() {
        fileText.clear();
    }

    Scene SceneFile::loadScene(RenderAllocator &alloc, MonoCppRuntime &monoRuntime) {
        return parseJsonScene(fileText, alloc, monoRuntime);
    }
}
