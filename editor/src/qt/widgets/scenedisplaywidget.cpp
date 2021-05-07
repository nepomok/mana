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
#include <utility>

using namespace mana;

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

    doneCurrent();
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

void SceneDisplayWidget::setViewerType(mana::CameraType cameraType) {
    camType = cameraType;
}

mana::Transform &SceneDisplayWidget::getViewerTransform() {
    return viewerTransform;
}

void SceneDisplayWidget::setHighlightColor(mana::ColorRGB color) {
    highlightColor = color;
}

mana::ColorRGB SceneDisplayWidget::getHighlightColor() {
    return highlightColor;
}

void SceneDisplayWidget::setHighlightScale(float scale) {
    highlightScale = scale;
}

float SceneDisplayWidget::getHighlightScale() {
    return highlightScale;
}

void SceneDisplayWidget::setHighlightedNodes(const std::vector<std::string> &nodeNames) {
    highlightedNodes = nodeNames;
}

const std::vector<std::string> &SceneDisplayWidget::getHighlightedNodes() {
    return highlightedNodes;
}

mana::Renderer &SceneDisplayWidget::getRenderer() {
    return *ren;
}

mana::RenderAllocator &SceneDisplayWidget::getAllocator() {
    return *alloc;
}

void SceneDisplayWidget::setFps(int f) {
    fps = f;
}

int SceneDisplayWidget::getFps() {
    return fps;
}

void SceneDisplayWidget::setViewerMovementSpeed(float speed) {
    movSpeed = speed;
}

float SceneDisplayWidget::getViewerMovementSpeed() {
    return movSpeed;
}

void SceneDisplayWidget::setViewerRotationSpeed(float speed) {
    rotSpeed = speed;
}

float SceneDisplayWidget::getViewerRotationSpeed() {
    return rotSpeed;
}

void SceneDisplayWidget::setViewerInputMovement(const mana::Vec3f &movement) {
    inputMovement = movement;
}

const mana::Vec3f &SceneDisplayWidget::getViewerInputMovement() {
    return inputMovement;
}

void SceneDisplayWidget::setViewerInputRotation(const mana::Vec3f &rotation) {
    inputRotation = rotation;
}

const mana::Vec3f &SceneDisplayWidget::getViewerInputRotation() {
    return inputRotation;
}

void SceneDisplayWidget::onTimerUpdate() {
    float deltaTime = (float) delta.restart() / 1000;

    mana::Mat4f mat = MatrixMath::inverse(MatrixMath::rotate(viewerTransform.rotation));
    Vec4f tmp = mat * Vec4f(0, 0, 1, 0);
    Vec3f forward(tmp.x, tmp.y, tmp.z);
    tmp = mat * Vec4f(1, 0, 0, 0);
    Vec3f left(tmp.x, tmp.y, tmp.z);
    tmp = mat * Vec4f(0, 1, 0, 0);
    Vec3f up(tmp.x, tmp.y, tmp.z);

    viewerTransform.position += forward * movSpeed * deltaTime * inputMovement.z;
    viewerTransform.position += left * movSpeed * deltaTime * inputMovement.x;
    viewerTransform.position += up * movSpeed * deltaTime * inputMovement.y;

    viewerTransform.rotation += Vec3f(1, 0, 0) * rotSpeed * deltaTime * inputRotation.x;
    viewerTransform.rotation += Vec3f(0, 1, 0) * rotSpeed * deltaTime * inputRotation.y;
    viewerTransform.rotation += Vec3f(0, 0, 1) * rotSpeed * deltaTime * inputRotation.z;

    update();
}

struct RenderData {
    RenderData() = default;

    RenderData(std::string name, Node *node) : name(std::move(name)), node(node) {}

    std::string name;
    Node *node;
};

void SceneDisplayWidget::paintGL() {
    ren->initializeOpenGLFunctions();
    alloc->initializeOpenGLFunctions();

    ren->setClearColor(mana::ColorRGBA(38, 38, 38, 255));
    ren->setViewport({}, {frameBuffer.width, frameBuffer.height});

    RenderScene scene3d;

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


    std::vector<RenderData> data;
    for (auto &nodePointer : scene.nodes) {
        auto &node = nodePointer.second;
        if (!node.enabled)
            continue;

        if (!node.hasComponent<mana::RenderComponent>())
            continue;;

        auto &comp = node.getComponent<mana::RenderComponent>();
        if (!comp.enabled)
            continue;

        auto &tcomp = node.getComponent<mana::TransformComponent>();
        if (!tcomp.enabled)
            continue;

        data.emplace_back(RenderData(nodePointer.first, &node));
    }

    std::sort(data.begin(), data.end(),
              [](const RenderData &a, const RenderData &b) -> bool {
                  return a.node->getComponent<RenderComponent>().renderOrder <
                         b.node->getComponent<RenderComponent>().renderOrder;
              });

    for (auto &p : data) {
        RenderUnit unit;

        auto *comp = &p.node->getComponent<RenderComponent>();

        unit.transform = p.node->getComponent<TransformComponent>().transform;

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

        if (std::find(highlightedNodes.begin(), highlightedNodes.end(), p.name) != highlightedNodes.end()) {
            unit.outline = true;
            unit.outlineScale = highlightScale;
            unit.outlineColor = highlightColor;
        }

        scene3d.units.emplace_back(unit);
    }

    pCam.aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    if (camType == mana::PERSPECTIVE)
        scene3d.camera = &pCam;
    else
        scene3d.camera = &oCam;

    scene3d.camera->transform = viewerTransform;

    ren3d.render(frameBuffer, scene3d);

    QOpenGLWidget::paintGL();
}

void SceneDisplayWidget::resizeGL(int w, int h) {
    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.width = w;
    frameBuffer.height = h;
    QOpenGLWidget::resizeGL(w, h);
}

void SceneDisplayWidget::keyPressEvent(QKeyEvent *event) {
    Vec3f movInput = getViewerInputMovement();
    Vec3f rotInput = getViewerInputRotation();
    switch (event->key()) {
        case Qt::Key_W:
            movInput.z = -1;
            break;
        case Qt::Key_S:
            movInput.z = 1;
            break;
        case Qt::Key_A:
            movInput.x = -1;
            break;
        case Qt::Key_D:
            movInput.x = 1;
            break;
        case Qt::Key_Q:
            movInput.y = -1;
            break;
        case Qt::Key_E:
            movInput.y = 1;
            break;
        case Qt::Key_Up:
            rotInput.x = 1;
            break;
        case Qt::Key_Down:
            rotInput.x = -1;
            break;
        case Qt::Key_Left:
            rotInput.y = -1;
            break;
        case Qt::Key_Right:
            rotInput.y = 1;
            break;
    }
    setViewerInputMovement(movInput);
    setViewerInputRotation(rotInput);

    QWidget::keyPressEvent(event);
}

void SceneDisplayWidget::keyReleaseEvent(QKeyEvent *event) {
    Vec3f movInput = getViewerInputMovement();
    Vec3f rotInput = getViewerInputRotation();
    switch (event->key()) {
        case Qt::Key_W:
            if (movInput.z == -1)
                movInput.z = 0;
            break;
        case Qt::Key_S:
            if (movInput.z == 1)
                movInput.z = 0;
            break;
        case Qt::Key_A:
            if (movInput.x == -1)
                movInput.x = 0;
            break;
        case Qt::Key_D:
            if (movInput.x == 1)
                movInput.x = 0;
            break;
        case Qt::Key_Q:
            if (movInput.y == -1)
                movInput.y = 0;
            break;
        case Qt::Key_E:
            if (movInput.y == 1)
                movInput.y = 0;
            break;
        case Qt::Key_Up:
            if (rotInput.x == 1)
                rotInput.x = 0;
            break;
        case Qt::Key_Down:
            if (rotInput.x == -1)
                rotInput.x = 0;
            break;
        case Qt::Key_Left:
            if (rotInput.y == -1)
                rotInput.y = 0;
            break;
        case Qt::Key_Right:
            if (rotInput.y == 1)
                rotInput.y = 0;
            break;
    }
    setViewerInputMovement(movInput);
    setViewerInputRotation(rotInput);

    QWidget::keyReleaseEvent(event);
}
