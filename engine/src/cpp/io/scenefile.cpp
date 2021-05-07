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

#include "extern/json.hpp"

namespace mana {
    ComponentType convertComponentType(const std::string &str) {
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

    DepthTestMode convertDepthTestMode(const std::string &str) {
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

    StencilMode convertStencilMode(const std::string &str) {
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

    StencilAction convertStencilAction(const std::string &str) {
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

    FaceCullingMode convertFaceCullMode(const std::string &str) {
        if (str == "cull_none")
            return CULL_NONE;
        else if (str == "cull_front")
            return CULL_FRONT;
        else if (str == "cull_back")
            return CULL_BACK;
        else
            throw std::runtime_error("Invalid face cull mode " + str);
    }

    BlendMode convertBlendMode(const std::string &str) {
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

    CameraComponent *getCameraComponent(const nlohmann::json &component) {
        auto *ret = new CameraComponent();
        ret->cameraType = convertCameraType(component["cameraType"]);
        if (ret->cameraType == PERSPECTIVE) {
            ret->nearClip = component["nearClip"];
            ret->farClip = component["farClip"];
            ret->fov = component["fov"];
            ret->aspectRatio = component["aspectRatio"];
        } else {
            ret->nearClip = component["nearClip"];
            ret->farClip = component["farClip"];
            ret->left = component["left"];
            ret->right = component["right"];
            ret->top = component["top"];
            ret->bottom = component["bottom"];
        }
        return ret;
    }

    TransformComponent *getTransformComponent(const nlohmann::json &component) {
        auto *ret = new TransformComponent();
        ret->transform = convertTransform(component);
        return ret;
    }

    RenderComponent *getRenderComponent(const nlohmann::json &component, Resources &res) {
        auto *ret = new RenderComponent();
        ret->shader = &res.getResource<ShaderResource>(component["shaderResourceName"]);

        for (const auto &entry : component["textureMapping"]) {
            ret->textureMapping.insert({std::string(entry["name"]), entry["slot"]});
        }

        for (auto &mesh : component["meshes"]) {
            ret->meshBuffers.emplace_back(&res.getResource<MeshBufferResource>(mesh["resourceName"]));
        }

        for (const auto &tex : component["textures"]) {
            ret->textureBuffers.emplace_back(&res.getResource<TextureBufferResource>(tex["resourceName"]));
        }

        auto props = component["renderProperties"];
        ret->renderProperties.enableDepthTest = props["enableDepthTest"];
        ret->renderProperties.depthTestWrite = props["depthTestWrite"];
        ret->renderProperties.depthTestMode = convertDepthTestMode(props["depthTestMode"]);
        ret->renderProperties.enableStencilTest = props["enableStencilTest"];
        ret->renderProperties.stencilTestMask = props["stencilTestMask"];
        ret->renderProperties.stencilMode = convertStencilMode(props["stencilMode"]);
        ret->renderProperties.stencilReference = props["stencilReference"];
        ret->renderProperties.stencilFunctionMask = props["stencilFunctionMask"];
        ret->renderProperties.stencilFail = convertStencilAction(props["stencilFail"]);
        ret->renderProperties.stencilDepthFail = convertStencilAction(props["stencilDepthFail"]);
        ret->renderProperties.stencilPass = convertStencilAction(props["stencilPass"]);
        ret->renderProperties.enableFaceCulling = props["enableFaceCulling"];
        ret->renderProperties.faceCullMode = convertFaceCullMode(props["faceCullMode"]);
        ret->renderProperties.faceCullClockwiseWinding = props["faceCullClockwiseWinding"];
        ret->renderProperties.enableBlending = props["enableBlending"];
        ret->renderProperties.blendSourceMode = convertBlendMode(props["blendSourceMode"]);
        ret->renderProperties.blendDestinationMode = convertBlendMode(props["blendDestinationMode"]);

        ret->renderOrder = props["renderOrder"];

        return ret;
    }

    LightComponent *getLightComponent(const nlohmann::json &component) {
        auto *ret = new LightComponent();
        ret->lightType = convertLightType(component["lightType"]);

        ret->ambient.x = component["ambient"]["r"];
        ret->ambient.y = component["ambient"]["g"];
        ret->ambient.z = component["ambient"]["b"];
        ret->diffuse.x = component["diffuse"]["r"];
        ret->diffuse.y = component["diffuse"]["g"];
        ret->diffuse.z = component["diffuse"]["b"];
        ret->specular.x = component["specular"]["r"];
        ret->specular.y = component["specular"]["g"];
        ret->specular.z = component["specular"]["b"];

        switch (ret->lightType) {
            case LIGHT_POINT:
                ret->constant = component["constant"];
                ret->linear = component["linear"];
                ret->quadratic = component["quadratic"];
                break;
            case LIGHT_SPOT:
                ret->direction.x = component["direction"]["x"];
                ret->direction.y = component["direction"]["y"];
                ret->direction.z = component["direction"]["z"];
                ret->cutOff = component["cutOff"];
                ret->outerCutOff = component["outerCutOff"];
                ret->constant = component["constant"];
                ret->linear = component["linear"];
                ret->quadratic = component["quadratic"];
                break;
            case LIGHT_DIRECTIONAL:
                ret->direction.x = component["direction"]["x"];
                ret->direction.y = component["direction"]["y"];
                ret->direction.z = component["direction"]["z"];
                break;
        }
        return ret;
    }

    ScriptComponent *getScriptComponent(const nlohmann::json &component, Resources &res) {
        auto *ret = new ScriptComponent();
        ret->script = &res.getResource<ScriptResource>(component["resourceName"]);
        ret->queue = component["queue"];
        return ret;
    }

    Component *getComponent(const nlohmann::json &component, Resources &res) {
        Component *ret;
        switch (convertComponentType(component["componentType"])) {
            case TRANSFORM:
                ret = getTransformComponent(component);
                break;
            case CAMERA:
                ret = getCameraComponent(component);
                break;
            case RENDER:
                ret = getRenderComponent(component, res);
                break;
            case LIGHT:
                ret = getLightComponent(component);
                break;
            case SCRIPT:
                ret = getScriptComponent(component, res);
                break;
            default:
                throw std::runtime_error("Unrecognized component type");
        }
        ret->enabled = component["enabled"];
        return ret;
    }

    Node getNode(const nlohmann::json &node, Resources &res) {
        Node ret;
        ret.enabled = node["enabled"];
        for (auto &comp : node["components"]) {
            ret.addComponentPointer(getComponent(comp, res));
        }
        return ret;
    }

    Scene getSceneFromJson(const std::string &jsonText, Resources &res) {
        Scene ret;
        auto j = nlohmann::json::parse(jsonText);
        for (auto &node : j["nodes"]) {
            std::string nodeName = node["nodeName"];
            if (ret.nodes.find(nodeName) != ret.nodes.end())
                throw std::runtime_error("Duplicate node name");
            ret.nodes[node["nodeName"]] = getNode(node, res);
        }
        return ret;
    }

    SceneFile::SceneFile(const std::string &fp) {
        filePath = fp;
        sceneJsonSource = File::readAllText(filePath);
        auto json = nlohmann::json::parse(sceneJsonSource);
        sceneName = json["sceneName"];
        sceneResources = json["sceneResources"];
    }

    void SceneFile::open() {
        sceneJsonSource = File::readAllText(filePath);
        auto json = nlohmann::json::parse(sceneJsonSource);
        sceneName = json["sceneName"];
        sceneResources = json["sceneResources"];
        File::open();
    }

    void SceneFile::close() {
        sceneJsonSource.clear();
        sceneName.clear();
        sceneResources.clear();
        File::close();
    }

    const std::string &SceneFile::getSceneName() {
        return sceneName;
    }

    const std::string &SceneFile::getSceneResourcesName() {
        return sceneResources;
    }

    Scene SceneFile::getScene(Resources &res) {
        return getSceneFromJson(sceneJsonSource, res);
    }
}