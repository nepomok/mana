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

#include <algorithm>

#include "engine/ecs/systems/rendersystem.hpp"

#include "engine/ecs/components.hpp"

#include "engine/render/camera/perspectivecamera.hpp"
#include "engine/render/camera/orthographiccamera.hpp"

namespace mana {
    RenderSystem::RenderSystem(const RenderTarget &scr, Renderer3D &ren, Resources &res)
            : screenTarget(scr), ren(ren), res(res) {
    }

    void RenderSystem::start() {

    }

    void RenderSystem::stop() {

    }

    void RenderSystem::update(float deltaTime, Scene &scene) {
        LightingData lights;

        auto nodes = scene.findNodesWithComponent<LightComponent>();
        for (auto *nodePointer : nodes) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto comp = node.getComponent<LightComponent>();
            if (!comp.enabled)
                continue;

            if (comp.lightType == LIGHT_DIRECTIONAL) {
                DirectionalLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.direction = comp.direction;

                lights.dir.emplace_back(light);
            } else if (comp.lightType == LIGHT_POINT) {
                PointLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.constant = comp.constant;
                light.linear = comp.linear;
                light.quadratic = comp.quadratic;

                lights.point.emplace_back(light);
            } else if (comp.lightType == LIGHT_SPOT) {
                SpotLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.cutOff = comp.cutOff;
                light.outerCutOff = comp.outerCutOff;
                light.constant = comp.constant;
                light.linear = comp.linear;
                light.quadratic = comp.quadratic;

                lights.spot.emplace_back(light);
            }
        }

        std::map<RenderComponent *, TransformComponent *> mapping;
        std::vector<RenderComponent *> renderComponents;
        nodes = scene.findNodesWithComponent<RenderComponent>();
        for (auto &nodePointer : nodes) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &comp = node.getComponent<RenderComponent>();
            if (!comp.enabled)
                continue;

            auto &tcomp = node.getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            renderComponents.emplace_back(&comp);
            mapping[&comp] = &tcomp;
        }

        std::sort(renderComponents.begin(), renderComponents.end(),
                  [](const RenderComponent *a, const RenderComponent *b) -> bool {
                      return a->renderOrder < b->renderOrder;
                  });

        std::vector<RenderCommand> commands;
        for (auto *comp : renderComponents) {
            RenderCommand command;

            command.transform = mapping[comp]->transform;

            command.shader = res.getResource<ShaderResource>(comp->shaderResourceName).getShader();
            for (auto &m : comp->textureMapping) {
                command.shader->setTexture(m.first, m.second);
            }

            for (auto &t : comp->textureResourceNames) {
                command.textureObjects.emplace_back(res.getResource<TextureResource>(t).getTexture());
            }

            for (auto &m : comp->meshResourceNames) {
                command.meshObjects.emplace_back(res.getResource<MeshBufferResource>(m).getRenderMesh());
            }

            command.properties.enableDepthTest = comp->renderProperties.enableDepthTest;
            command.properties.depthTestWrite = comp->renderProperties.depthTestWrite;
            command.properties.depthTestMode = comp->renderProperties.depthTestMode;

            command.properties.enableStencilTest = comp->renderProperties.enableStencilTest;
            command.properties.stencilTestMask = comp->renderProperties.stencilTestMask;
            command.properties.stencilMode = comp->renderProperties.stencilMode;
            command.properties.stencilReference = comp->renderProperties.stencilReference;
            command.properties.stencilFunctionMask = comp->renderProperties.stencilFunctionMask;
            command.properties.stencilFail = comp->renderProperties.stencilFail;
            command.properties.stencilDepthFail = comp->renderProperties.stencilDepthFail;
            command.properties.stencilPass = comp->renderProperties.stencilPass;

            command.properties.enableFaceCulling = comp->renderProperties.enableFaceCulling;
            command.properties.faceCullMode = comp->renderProperties.faceCullMode;
            command.properties.faceCullClockwiseWinding = comp->renderProperties.faceCullClockwiseWinding;

            command.properties.enableBlending = comp->renderProperties.enableBlending;
            command.properties.blendSourceMode = comp->renderProperties.blendSourceMode;
            command.properties.blendDestinationMode = comp->renderProperties.blendDestinationMode;

            commands.emplace_back(command);
        }

        nodes = scene.findNodesWithComponent<CameraComponent>();
        Node *cameraNode;
        for (auto &node : nodes) {
            if (!node->enabled)
                continue;
            auto &comp = node->getComponent<CameraComponent>();
            if (!comp.enabled)
                continue;
            auto &tcomp = node->getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;
            cameraNode = node;
            break;
        }

        auto &comp = cameraNode->getComponent<CameraComponent>();
        auto &tcomp = cameraNode->getComponent<TransformComponent>();

        if (comp.cameraType == PERSPECTIVE) {
            PerspectiveCamera cam;
            cam.transform = tcomp.transform;
            cam.nearClip = comp.nearClip;
            cam.farClip = comp.farClip;
            cam.fov = comp.fov;
            cam.aspectRatio = comp.aspectRatio;
            ren.render(screenTarget, cam, commands, lights);
        } else {
            OrthographicCamera cam;
            cam.transform = tcomp.transform;
            cam.nearClip = comp.nearClip;
            cam.farClip = comp.farClip;
            cam.left = comp.left;
            cam.top = comp.top;
            cam.right = comp.right;
            cam.bottom = comp.bottom;
            ren.render(screenTarget, cam, commands, lights);
        }
    }
}