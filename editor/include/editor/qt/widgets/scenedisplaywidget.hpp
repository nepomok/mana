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

#include <QOpenGLWidget>
#include <QTimer>
#include <QElapsedTimer>

#include "mana.hpp"

#include "opengl/qtoglrenderer.hpp"
#include "opengl/qtoglrenderdevice.hpp"

#include "opengl/qtoglrendertarget.hpp"

//TODO: Deallocate render resources (The destructors have to be called from paintGl)
class SceneDisplayWidget : public QOpenGLWidget {
Q_OBJECT
public:
    explicit SceneDisplayWidget(QWidget *parent = nullptr, int fps = 60);

    ~SceneDisplayWidget() override;

    mana::RenderDevice &getDevice();

    void setFps(int fps);

    int getFps();

    void setScene(const mana::Scene &scene);

    mana::Scene &getScene();

    void setViewerType(mana::CameraType cameraType);

    mana::Transform &getViewerTransform();

    void setHighlightColor(mana::ColorRGB color);

    mana::ColorRGB getHighlightColor();

    void setHighlightScale(float scale);

    float getHighlightScale();

    void setHighlightedNodes(const std::vector<std::string> &nodeNames);

    const std::vector<std::string> &getHighlightedNodes();

    void setViewerMovementSpeed(float speed);

    float getViewerMovementSpeed();

    void setViewerRotationSpeed(float speed);

    float getViewerRotationSpeed();

    void setViewerInputMovement(const mana::Vec3f &movement);

    const mana::Vec3f &getViewerInputMovement();

    void setViewerInputRotation(const mana::Vec3f &rotation);

    const mana::Vec3f &getViewerInputRotation();

protected:
    void paintGL() override;

    void resizeGL(int w, int h) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private slots:

    void onTimerUpdate();

private:
    int fps;

    QTimer timer;

    QElapsedTimer delta;

    mana::opengl::QtOGLRenderDevice device;

    mana::Scene scene;

    mana::Vec3f inputMovement;
    mana::Vec3f inputRotation;

    mana::Transform viewerTransform;

    mana::CameraType camType = mana::CameraType::PERSPECTIVE;
    mana::Camera pCam;

    float movSpeed = 5;
    float rotSpeed = 50;

    mana::opengl::QtOGLRenderTarget frameBuffer;

    std::vector<std::string> highlightedNodes;

    mana::ColorRGB highlightColor = {10, 210, 50};

    float highlightScale = 1.01f;
};

#endif //MANA_SCENEDISPLAYWIDGET_HPP
