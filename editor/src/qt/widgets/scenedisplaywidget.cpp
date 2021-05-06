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

#include "editor/qt/widgets/scenedisplaywidget.hpp"

#include "opengl/qtoglrenderer.hpp"
#include "opengl/qtoglrenderallocator.hpp"
#include "opengl/qtogluserframebuffer.hpp"

#include <QResizeEvent>

SceneDisplayWidget::SceneDisplayWidget(QWidget *parent, int fps) : QOpenGLWidget(parent), fps(fps) {
    makeCurrent();

    frameBuffer.deleteFramebuffer = false;

    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.width = width();
    frameBuffer.height = height();

    ren = new mana::opengl::QtOGLRenderer();
    alloc = new mana::opengl::QtOGLRenderAllocator();
    ren3d = mana::Renderer3D(*ren, *alloc);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));

    timer.start((int) ((1.0f / (float) fps) * 1000));
}

SceneDisplayWidget::~SceneDisplayWidget() {
    delete alloc;
    delete ren;
}

void SceneDisplayWidget::setScene(const mana::Scene &s) {
    scene = s;
}

mana::Scene &SceneDisplayWidget::getScene() {
    return scene;
}

void SceneDisplayWidget::setViewer(mana::Camera *camera) {
    viewerCamera = camera;
}

mana::Renderer &SceneDisplayWidget::getRenderer() {
    return *ren;
}

mana::RenderAllocator &SceneDisplayWidget::getAllocator() {
    return *alloc;
}

void SceneDisplayWidget::onTimerUpdate() {
}

void SceneDisplayWidget::paintGL() {
    QOpenGLWidget::paintGL();
    ren->initializeOpenGLFunctions();
    alloc->initializeOpenGLFunctions();

    ren->setClearColor(mana::ColorRGBA(0, 0, 255, 255));
    ren->setViewport({}, {frameBuffer.width, frameBuffer.height});

    mana::Renderer3D::RenderScene scene3d;

    auto nodes = scene.findNodesWithComponent<mana::LightComponent>();
    for (auto *nodePointer : nodes) {
        auto &node = *nodePointer;
        if (!node.enabled)
            continue;

        auto comp = node.getComponent<mana::LightComponent>();
        if (!comp.enabled)
            continue;

        if (comp.lightType == mana::LIGHT_DIRECTIONAL) {
            mana::DirectionalLight light;
            light.ambient = comp.ambient;
            light.diffuse = comp.diffuse;
            light.specular = comp.specular;

            light.direction = comp.direction;

            scene3d.dir.emplace_back(light);
        } else if (comp.lightType == mana::LIGHT_POINT) {
            mana::PointLight light;
            light.ambient = comp.ambient;
            light.diffuse = comp.diffuse;
            light.specular = comp.specular;

            light.constant = comp.constant;
            light.linear = comp.linear;
            light.quadratic = comp.quadratic;

            scene3d.point.emplace_back(light);
        } else if (comp.lightType == mana::LIGHT_SPOT) {
            mana::SpotLight light;
            light.ambient = comp.ambient;
            light.diffuse = comp.diffuse;
            light.specular = comp.specular;

            light.cutOff = comp.cutOff;
            light.outerCutOff = comp.outerCutOff;
            light.constant = comp.constant;
            light.linear = comp.linear;
            light.quadratic = comp.quadratic;

            scene3d.spot.emplace_back(light);
        }
    }

    std::map<mana::RenderComponent *, mana::TransformComponent *> mapping;
    std::vector<mana::RenderComponent *> renderComponents;
    nodes = scene.findNodesWithComponent<mana::RenderComponent>();
    for (auto &nodePointer : nodes) {
        auto &node = *nodePointer;
        if (!node.enabled)
            continue;

        auto &comp = node.getComponent<mana::RenderComponent>();
        if (!comp.enabled)
            continue;

        auto &tcomp = node.getComponent<mana::TransformComponent>();
        if (!tcomp.enabled)
            continue;

        renderComponents.emplace_back(&comp);
        mapping[&comp] = &tcomp;
    }

    std::sort(renderComponents.begin(), renderComponents.end(),
              [](const mana::RenderComponent *a, const mana::RenderComponent *b) -> bool {
                  return a->renderOrder < b->renderOrder;
              });

    for (auto *comp : renderComponents) {
        mana::Renderer3D::Unit unit;

        unit.transform = mapping[comp]->transform;

        unit.command.shader = comp->shader->getShader();
        for (auto &m : comp->textureMapping) {
            unit.command.shader->setTexture(m.first, m.second);
        }

        for (auto *t : comp->textureBuffers) {
            unit.command.textures.emplace_back(t->getTextureBuffer());
        }

        for (auto *m : comp->meshBuffers) {
            unit.command.meshBuffers.emplace_back(m->getRenderMesh());
        }

        unit.command.properties.enableDepthTest = comp->renderProperties.enableDepthTest;
        unit.command.properties.depthTestWrite = comp->renderProperties.depthTestWrite;
        unit.command.properties.depthTestMode = comp->renderProperties.depthTestMode;

        unit.command.properties.enableStencilTest = comp->renderProperties.enableStencilTest;
        unit.command.properties.stencilTestMask = comp->renderProperties.stencilTestMask;
        unit.command.properties.stencilMode = comp->renderProperties.stencilMode;
        unit.command.properties.stencilReference = comp->renderProperties.stencilReference;
        unit.command.properties.stencilFunctionMask = comp->renderProperties.stencilFunctionMask;
        unit.command.properties.stencilFail = comp->renderProperties.stencilFail;
        unit.command.properties.stencilDepthFail = comp->renderProperties.stencilDepthFail;
        unit.command.properties.stencilPass = comp->renderProperties.stencilPass;

        unit.command.properties.enableFaceCulling = comp->renderProperties.enableFaceCulling;
        unit.command.properties.faceCullMode = comp->renderProperties.faceCullMode;
        unit.command.properties.faceCullClockwiseWinding = comp->renderProperties.faceCullClockwiseWinding;

        unit.command.properties.enableBlending = comp->renderProperties.enableBlending;
        unit.command.properties.blendSourceMode = comp->renderProperties.blendSourceMode;
        unit.command.properties.blendDestinationMode = comp->renderProperties.blendDestinationMode;

        scene3d.units.emplace_back(unit);
    }

    scene3d.camera = viewerCamera;

    if (viewerCamera != nullptr && viewerCamera->type == mana::PERSPECTIVE) {
        dynamic_cast<mana::PerspectiveCamera *>(viewerCamera)->aspectRatio =
                static_cast<float>(width()) / static_cast<float>(height());
    }

    ren3d.render(frameBuffer, scene3d);
}

void SceneDisplayWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.width = w;
    frameBuffer.height = h;
}
