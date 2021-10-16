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

#include <QResizeEvent>
#include <utility>

#include "qt/opengl/qtoglrenderdevice.hpp"

using namespace engine;

SceneDisplayWidget::SceneDisplayWidget(QWidget *parent, int fps) : fps(fps) {
}

SceneDisplayWidget::~SceneDisplayWidget() = default;

void SceneDisplayWidget::setScene(const engine::Scene &s) {
}

engine::Scene &SceneDisplayWidget::getScene() {
}

void SceneDisplayWidget::setViewerType(engine::CameraType cameraType) {
    camType = cameraType;
}

engine::Transform &SceneDisplayWidget::getViewerTransform() {
    return viewerTransform;
}

void SceneDisplayWidget::setHighlightColor(engine::ColorRGB color) {
    highlightColor = color;
}

engine::ColorRGB SceneDisplayWidget::getHighlightColor() {
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

engine::RenderDevice &SceneDisplayWidget::getDevice() {
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

void SceneDisplayWidget::setViewerInputMovement(const engine::Vec3f &movement) {
    inputMovement = movement;
}

const engine::Vec3f &SceneDisplayWidget::getViewerInputMovement() {
    return inputMovement;
}

void SceneDisplayWidget::setViewerInputRotation(const engine::Vec3f &rotation) {
    inputRotation = rotation;
}

const engine::Vec3f &SceneDisplayWidget::getViewerInputRotation() {
    return inputRotation;
}

void SceneDisplayWidget::onTimerUpdate() {

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
