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
#include "opengl/qtoglrenderdevice.hpp"
#include "opengl/qtoglrendertarget.hpp"

#include <QResizeEvent>
#include <utility>

using namespace mana;

SceneDisplayWidget::SceneDisplayWidget(QWidget *parent, int fps) : QOpenGLWidget(parent), fps(fps) {
    makeCurrent();

    frameBuffer.deleteFramebuffer = false;

    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.size = {width(), height()};

    ren3d = std::move(mana::Renderer3D(device, {}));

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));

    timer.start((int) ((1.0f / (float) fps) * 1000));

    doneCurrent();
}

SceneDisplayWidget::~SceneDisplayWidget() = default;

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

mana::RenderDevice &SceneDisplayWidget::getDevice() {
    return device;
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
    device.initializeOpenGLFunctions();

    device.getRenderer().setClearColor(mana::ColorRGBA(38, 38, 38, 255));
    device.getRenderer().setViewport({}, frameBuffer.size);

    RenderScene scene3d;

    for (auto *nodePointer : scene.findNodesWithComponent<LightComponent>()) {
        auto &node = *nodePointer;
        if (!node.enabled)
            continue;

        auto lightComponent = node.getComponent<LightComponent>();
        if (!lightComponent.enabled)
            continue;

        scene3d.lights.emplace_back(lightComponent.light);
    }

    for (auto &nodePointer : scene.findNodesWithComponent<RenderCommand>()) {
        auto &node = *nodePointer;
        if (!node.enabled)
            continue;

        auto &transformComponent = node.getComponent<TransformComponent>();
        if (!transformComponent.enabled)
            continue;

        auto &meshComponent = node.getComponent<MeshComponent>();
        if (!meshComponent.enabled)
            continue;

        auto &materialComponent = node.getComponent<MaterialComponent>();
        if (!materialComponent.enabled)
            continue;

        RenderUnit unit;
        unit.transform = TransformComponent::walkTransformHierarchy(transformComponent);
        unit.material = materialComponent.material;
        for (auto *m : meshComponent.meshes) {
            unit.meshes.emplace_back(m);
        }

        scene3d.forward.emplace_back(unit);
    }

    Node *cameraNode;
    for (auto &node : scene.findNodesWithComponent<CameraComponent>()) {
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

    auto &cameraComponent = cameraNode->getComponent<CameraComponent>();
    auto &cameraTransformComponent = cameraNode->getComponent<TransformComponent>();

    scene3d.camera = cameraComponent.camera;
    scene3d.camera.transform = TransformComponent::walkTransformHierarchy(cameraTransformComponent);

    scene3d.camera.aspectRatio = static_cast<float>(width()) / static_cast<float>(height());

    ren3d.render(frameBuffer, scene3d);

    QOpenGLWidget::paintGL();
}

void SceneDisplayWidget::resizeGL(int w, int h) {
    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.size = {w, h};
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
