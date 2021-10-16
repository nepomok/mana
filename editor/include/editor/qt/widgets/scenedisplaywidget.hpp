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

#ifndef MANA_SCENEDISPLAYWIDGET_HPP
#define MANA_SCENEDISPLAYWIDGET_HPP

#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>

#include <QOpenGLWidget>
#include <QOffscreenSurface>

#include "engine.hpp"

class SceneDisplayWidget : public QOpenGLWidget {
Q_OBJECT
public:
    explicit SceneDisplayWidget(QWidget *parent = nullptr, int fps = 60);

    ~SceneDisplayWidget() override;

    engine::RenderDevice &getDevice();

    void setFps(int fps);

    int getFps();

    void setScene(const engine::Scene &scene);

    engine::Scene &getScene();

    void setViewerType(engine::CameraType cameraType);

    engine::Transform &getViewerTransform();

    void setHighlightColor(engine::ColorRGB color);

    engine::ColorRGB getHighlightColor();

    void setHighlightScale(float scale);

    float getHighlightScale();

    void setHighlightedNodes(const std::vector<std::string> &nodeNames);

    const std::vector<std::string> &getHighlightedNodes();

    void setViewerMovementSpeed(float speed);

    float getViewerMovementSpeed();

    void setViewerRotationSpeed(float speed);

    float getViewerRotationSpeed();

    void setViewerInputMovement(const engine::Vec3f &movement);

    const engine::Vec3f &getViewerInputMovement();

    void setViewerInputRotation(const engine::Vec3f &rotation);

    const engine::Vec3f &getViewerInputRotation();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private slots:

    void onTimerUpdate();

private:
    int fps;

    QTimer timer;

    QElapsedTimer delta;

    engine::EntityManager scene;

    engine::Vec3f inputMovement;
    engine::Vec3f inputRotation;

    engine::Transform viewerTransform;

    engine::CameraType camType = engine::CameraType::PERSPECTIVE;
    engine::Camera pCam;

    float movSpeed = 5;
    float rotSpeed = 50;

    std::vector<std::string> highlightedNodes;
    engine::ColorRGB highlightColor = {10, 210, 50};
    float highlightScale = 1.01f;
};

#endif //MANA_SCENEDISPLAYWIDGET_HPP
