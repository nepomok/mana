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

SceneDisplayWidget::SceneDisplayWidget(int fps) : fps(fps) {
    makeCurrent();
    ren = mana::Renderer::instantiate(mana::OPENGL);
    alloc = mana::RenderAllocator::instantiate(mana::OPENGL);
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
