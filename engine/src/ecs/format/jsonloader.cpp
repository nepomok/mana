/**
 *  Mana - 3D Engine
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

#include "engine/ecs/format/jsonloader.hpp"
#include "engine/ecs/components/cameracomponent.hpp"
#include "engine/ecs/components/lightcomponent.hpp"
#include "engine/ecs/components/scriptcomponent.hpp"
#include "engine/ecs/components/transformcomponent.hpp"
#include "engine/ecs/components/rendercomponent.hpp"

#include "engine/io/imageloader.hpp"
#include "engine/io/meshloader.hpp"
#include "engine/io/file.hpp"

#include "engine/render/camera/perspectivecamera.hpp"
#include "engine/render/camera/orthographiccamera.hpp"
#include "engine/render/renderer3d.hpp"

#include "extern/json.hpp"

namespace mana {
    namespace JsonLoader {
        CameraComponent getCamera(const nlohmann::json &component) {
            CameraComponent ret;
            if (component["cameraType"] == PERSPECTIVE) {
                auto *cam = new PerspectiveCamera();
                cam->nearClip = component["nearClip"];
                cam->farClip = component["farClip"];
                cam->fov = component["fov"];
                cam->aspectRatio = component["aspectRatio"];
                ret.camera = cam;
            } else {
                auto *cam = new OrthographicCamera();
                cam->nearClip = component["nearClip"];
                cam->farClip = component["farClip"];
                cam->left = component["left"];
                cam->right = component["right"];
                cam->top = component["top"];
                cam->bottom = component["bottom"];
                ret.camera = cam;
            }
            return ret;
        }

        TransformComponent getTransform(const nlohmann::json &component) {
            TransformComponent ret;
            ret.transform.position.x = component["position.x"];
            ret.transform.position.y = component["position.y"];
            ret.transform.position.z = component["position.z"];
            ret.transform.rotation.x = component["rotation.x"];
            ret.transform.rotation.y = component["rotation.y"];
            ret.transform.rotation.z = component["rotation.z"];
            ret.transform.scale.x = component["scale.x"];
            ret.transform.scale.y = component["scale.y"];
            ret.transform.scale.z = component["scale.z"];
            return ret;
        }

        RenderComponent getRenderComponent(const nlohmann::json &component, RenderAllocator &alloc) {
            RenderComponent ret;
            auto vs = Renderer3D::preprocessGlsl(File::readAllText(component["vertexShaderPath"]));
            auto fs = Renderer3D::preprocessGlsl(File::readAllText(component["fragmentShaderPath"]));
            ret.shader = alloc.allocateShaderProgram(vs, fs);
            for (auto &meshPath : component["meshPaths"]) {
                auto mesh = MeshLoader::load(meshPath);
                ret.meshData.emplace_back(alloc.allocateMesh(mesh));
            }
            for (auto &texturePath : component["texturePaths"]) {
                auto img = ImageLoader::load(texturePath);
                auto attr = RenderTexture::Attributes();
                attr.size = img.getSize();
                auto tex = alloc.allocateTexture(attr);
                tex->upload(img);
                ret.textures.emplace_back(tex);
            }

            return ret;
        }

        Scene import(std::string jsonStr, RenderAllocator &allocator) {
            nlohmann::json j = nlohmann::json::parse(jsonStr);
            Scene ret;
            for (auto &node : j["nodes"]) {
                std::string nodeName = node["nodeName"];
                if (ret.nodes.find(nodeName) != ret.nodes.end())
                    throw std::runtime_error("Node with name " + nodeName + " already exists.");
                Node n;
                for (auto &component : node["components"]) {
                    switch (component["type"].get<ComponentType>()) {
                        case TRANSFORM:
                            n.addComponent(getTransform(component));
                            break;
                        case CAMERA:
                            n.addComponent(getCamera(component));
                            break;
                        case RENDER:
                            n.addComponent(getRenderComponent(component, allocator));
                            break;
                        case LIGHT:
                        case SCRIPT:
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
}