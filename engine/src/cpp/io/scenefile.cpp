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
#include "engine/render/rendertexture.hpp"

#include "extern/json.hpp"

namespace mana {
    RenderTexture::TextureType parseTextureType(const std::string &str) {
        if (str == "texture2d")
            return RenderTexture::TEXTURE_2D;
        else if (str == "cubemap")
            return RenderTexture::TEXTURE_CUBE_MAP;
        else
            throw std::runtime_error("Invalid texture type " + str);
    }

    RenderTexture::ColorFormat parseColorFormat(const std::string &str) {
        if (str == "rgba")
            return RenderTexture::RGBA;
        else if (str == "rgb")
            return RenderTexture::RGB;
        else
            throw std::runtime_error("Invalid color format " + str);
    }

    RenderTexture::TextureWrapping parseTextureWrapping(const std::string &str) {
        if (str == "repeat")
            return RenderTexture::REPEAT;
        else if (str == "mirrored_repeat")
            return RenderTexture::MIRRORED_REPEAT;
        else if (str == "clamp_to_edge")
            return RenderTexture::CLAMP_TO_EDGE;
        else if (str == "clamp_to_border")
            return RenderTexture::CLAMP_TO_BORDER;
        else
            throw std::runtime_error("Invalid texture wrapping " + str);
    }

    RenderTexture::TextureFiltering parseTextureFiltering(const std::string &str) {
        if (str == "nearest")
            return RenderTexture::NEAREST;
        else if (str == "linear")
            return RenderTexture::LINEAR;
        else
            throw std::runtime_error("Invalid texture filtering " + str);
    }

    RenderTexture::MipMapFiltering parseMipMapFiltering(const std::string &str) {
        if (str == "nearest_mipmap_nearest")
            return RenderTexture::NEAREST_MIPMAP_NEAREST;
        else if (str == "linear_mipmap_nearest")
            return RenderTexture::LINEAR_MIPMAP_NEAREST;
        else if (str == "nearest_mipmap_linear")
            return RenderTexture::NEAREST_MIPMAP_LINEAR;
        else if (str == "linear_mipmap_linear")
            return RenderTexture::LINEAR_MIPMAP_LINEAR;
        else
            throw std::runtime_error("Invalid mipmap filtering " + str);
    }

    ComponentType parseComponent(const std::string &str) {
        if (str == "transform")
            return TRANSFORM;
        else if (str == "render")
            return RENDER;
        else if (str == "camera")
            return CAMERA;
        else if (str == "light")
            return LIGHT;
        else if (str == "script")
            return SCRIPT;
        throw std::runtime_error("Invalid component type " + str);
    }

    CameraType parseCameraType(const std::string &str) {
        if (str == "perspective")
            return PERSPECTIVE;
        else if (str == "orthographic")
            return ORTHOGRAPHIC;
        throw std::runtime_error("Invalid camera type " + str);
    }

    LightType parseLightType(const std::string &str) {
        if (str == "directional")
            return LIGHT_DIRECTIONAL;
        else if (str == "point")
            return LIGHT_POINT;
        else if (str == "spot")
            return LIGHT_SPOT;
        throw std::runtime_error("Invalid light type " + str);
    }

    DepthTestMode parseDepthTestMode(const std::string &str) {
        if (str == "depth_test_always")
            return DEPTH_TEST_ALWAYS;
        else if (str == "dpeth_test_never")
            return DEPTH_TEST_NEVER;
        else if (str == "depth_test_less")
            return DEPTH_TEST_LESS;
        else if (str == "depth_test_equal")
            return DEPTH_TEST_EQUAL;
        else if (str == "depth_test_lequal")
            return DEPTH_TEST_LEQUAL;
        else if (str == "depth_test_greater")
            return DEPTH_TEST_GREATER;
        else if (str == "depth_test_notequal")
            return DEPTH_TEST_NOTEQUAL;
        else if (str == "depth_test_gequal")
            return DEPTH_TEST_GEQUAL;
        else
            throw std::runtime_error("Invalid depth test mode " + str);
    }

    StencilMode parseStencilMode(const std::string &str) {
        if (str == "stencil_never")
            return STENCIL_NEVER;
        else if (str == "stencil_less")
            return STENCIL_LESS;
        else if (str == "stencil_lequal")
            return STENCIL_LEQUAL;
        else if (str == "stencil_greater")
            return STENCIL_GREATER;
        else if (str == "stencil_gequal")
            return STENCIL_GEQUAL;
        else if (str == "stencil_equal")
            return STENCIL_EQUAL;
        else if (str == "stencil_notequal")
            return STENCIL_NOTEQUAL;
        else if (str == "stencil_always")
            return STENCIL_ALWAYS;
        else
            throw std::runtime_error("Invalid stencil mode " + str);
    }

    StencilAction parseStencilAction(const std::string &str) {
        if (str == "stencil_keep")
            return STENCIL_KEEP;
        else if (str == "stencil_zero")
            return STENCIL_ZERO;
        else if (str == "stencil_replace")
            return STENCIL_REPLACE;
        else if (str == "stencil_incr")
            return STENCIL_INCR;
        else if (str == "stencil_incr_wrap")
            return STENCIL_INCR_WRAP;
        else if (str == "stencil_decr")
            return STENCIL_DECR;
        else if (str == "stencil_decr_wrap")
            return STENCIL_DECR_WRAP;
        else if (str == "stencil_invert")
            return STENCIL_INVERT;
        else
            throw std::runtime_error("Invalid stencil action " + str);
    }

    FaceCullingMode parseFaceCullMode(const std::string &str) {
        if (str == "cull_none")
            return CULL_NONE;
        else if (str == "cull_front")
            return CULL_FRONT;
        else if (str == "cull_back")
            return CULL_BACK;
        else
            throw std::runtime_error("Invalid face cull mode " + str);
    }

    BlendMode parseBlendMode(const std::string &str) {
        if (str == "zero")
            return ZERO;
        else if (str == "one")
            return ONE;
        else if (str == "src_color")
            return SRC_COLOR;
        else if (str == "one_minus_src_color")
            return ONE_MINUS_SRC_COLOR;
        else if (str == "dst_color")
            return DST_COLOR;
        else if (str == "src_alpha")
            return SRC_ALPHA;
        else if (str == "one_minus_src_alpha")
            return ONE_MINUS_SRC_ALPHA;
        else if (str == "dst_alpha")
            return DST_ALPHA;
        else if (str == "one_minus_dst_alpha")
            return ONE_MINUS_DST_ALPHA;
        else if (str == "constant_color")
            return CONSTANT_COLOR;
        else if (str == "one_minus_constant_color")
            return ONE_MINUS_CONSTANT_COLOR;
        else if (str == "constant_alpha")
            return CONSTANT_ALPHA;
        else if (str == "one_minus_constant_alpha")
            return ONE_MINUS_CONSTANT_ALPHA;
        else
            throw std::runtime_error("Invalid blend mode " + str);
    }

    CameraComponent getCamera(const nlohmann::json &component) {
        CameraComponent ret;
        ret.cameraType = parseCameraType(component["cameraType"]);
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

    Transform getTransform(const nlohmann::json &component) {
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

    TransformComponent getTransformComponent(const nlohmann::json &component) {
        TransformComponent ret;
        ret.transform = getTransform(component);
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

        for (auto &mesh : component["meshes"]) {
            auto meshAsset = AssetFile(mesh["filePath"]);
            if (mesh["instanced"]) {
                std::vector<Transform> offsets;
                for (auto &t : mesh["offsets"]) {
                    offsets.emplace_back(getTransform(t));
                }
                ret.meshData.emplace_back(alloc.allocateInstancedMesh(meshAsset.getMesh(), offsets));
            } else {
                ret.meshData.emplace_back(alloc.allocateMesh(meshAsset.getMesh()));
            }
        }

        for (const auto &entry : component["textures"]) {
            auto img = ImageFile(entry["filePath"]);
            auto attr = RenderTexture::Attributes();
            attr.textureType = parseTextureType(entry["textureType"]);
            attr.format = parseColorFormat(entry["format"]);
            attr.wrapping = parseTextureWrapping(entry["wrapping"]);
            attr.filterMin = parseTextureFiltering(entry["filterMin"]);
            attr.filterMag = parseTextureFiltering(entry["filterMag"]);
            attr.mipmapFilter = parseMipMapFiltering(entry["mipmapFilter"]);
            attr.generateMipmap = entry["generateMipmap"];
            attr.size = img.getBuffer().getSize();

            if (attr.textureType == RenderTexture::TEXTURE_2D) {
                auto tex = alloc.allocateTexture(attr);
                tex->upload(img.getBuffer());
                ret.textures.emplace_back(tex);
            } else if (attr.textureType == RenderTexture::TEXTURE_CUBE_MAP) {
                attr.size.x = attr.size.x / 6;
                if (attr.size.x != attr.size.y)
                    throw std::runtime_error("Invalid cubemap image file");
                auto tex = alloc.allocateTexture(attr);
                tex->uploadCubeMap(img.getBuffer());
                ret.textures.emplace_back(tex);
            }
        }

        auto props = component["renderProperties"];
        ret.enableDepthTest = props["enableDepthTest"];
        ret.depthTestWrite = props["depthTestWrite"];
        ret.depthTestMode = parseDepthTestMode(props["depthTestMode"]);
        ret.enableStencilTest = props["enableStencilTest"];
        ret.stencilTestMask = props["stencilTestMask"];
        ret.stencilMode = parseStencilMode(props["stencilMode"]);
        ret.stencilReference = props["stencilReference"];
        ret.stencilFunctionMask = props["stencilFunctionMask"];
        ret.stencilFail = parseStencilAction(props["stencilFail"]);
        ret.stencilDepthFail = parseStencilAction(props["stencilDepthFail"]);
        ret.stencilPass = parseStencilAction(props["stencilPass"]);
        ret.enableFaceCulling = props["enableFaceCulling"];
        ret.faceCullMode = parseFaceCullMode(props["faceCullMode"]);
        ret.faceCullClockwiseWinding = props["faceCullClockwiseWinding"];
        ret.enableBlending = props["enableBlending"];
        ret.blendSourceMode = parseBlendMode(props["blendSourceMode"]);
        ret.blendDestinationMode = parseBlendMode(props["blendDestinationMode"]);

        ret.order = props["renderOrder"];

        return ret;
    }

    LightComponent getLight(const nlohmann::json &component) {
        LightComponent ret;
        ret.lightType = parseLightType(component["lightType"]);

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
                switch (parseComponent(component["componentType"])) {
                    case TRANSFORM:
                        n.addComponent(getTransformComponent(component));
                        break;
                    case CAMERA:
                        n.addComponent(getCamera(component));
                        break;
                    case RENDER:
                        n.addComponent(getRenderComponent(component, allocator));
                        break;
                    case LIGHT:
                        n.addComponent(getLight(component));
                        break;
                    case SCRIPT:
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
