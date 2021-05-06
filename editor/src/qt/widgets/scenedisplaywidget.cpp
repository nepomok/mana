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

void SceneDisplayWidget::setScene(const mana::Renderer3D::RenderScene &s) {
    scene = s;
}

mana::Renderer3D::RenderScene &SceneDisplayWidget::getScene() {
    return scene;
}

void SceneDisplayWidget::onTimerUpdate() {
}

void SceneDisplayWidget::paintGL() {
    QOpenGLWidget::paintGL();
    ren->initializeOpenGLFunctions();
    ren->setClearColor(mana::ColorRGBA(0, 0, 255, 255));
    ren->setViewport({}, {frameBuffer.width, frameBuffer.height});
    ren->renderBegin(frameBuffer);
    ren->renderFinish();
}

void SceneDisplayWidget::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    frameBuffer.FBO = defaultFramebufferObject();
    frameBuffer.width = w;
    frameBuffer.height = h;
}
