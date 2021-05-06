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

#include "engine/render/renderer3d.hpp"

#include "opengl/qtoglrenderer.hpp"
#include "opengl/qtoglrenderallocator.hpp"

#include "opengl/qtogluserframebuffer.hpp"

class SceneDisplayWidget : public QOpenGLWidget {
Q_OBJECT
public:
    explicit SceneDisplayWidget(QWidget *parent = nullptr, int fps = 30);

    ~SceneDisplayWidget() override;

    void setScene(const mana::Renderer3D::RenderScene &scene);

    mana::Renderer3D::RenderScene &getScene();

protected:
    void paintGL() override;

    void resizeGL(int w, int h) override;

private slots:

    void onTimerUpdate();

private:

    int fps;

    QTimer timer;

    mana::opengl::QtOGLRenderer *ren;
    mana::opengl::QtOGLRenderAllocator *alloc;
    mana::Renderer3D ren3d;
    mana::Renderer3D::RenderScene scene;

    mana::opengl::QtOGLUserFrameBuffer frameBuffer;
};

#endif //MANA_SCENEDISPLAYWIDGET_HPP
