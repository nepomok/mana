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

#include "editor/qt/mainwindow.hpp"

#include "engine/io/json/jsonscenefile.hpp"
#include "engine/io/json/jsonresourcefile.hpp"

#include <QVBoxLayout>
#include <QKeyEvent>

using namespace mana;

MainWindow::MainWindow() {
    sceneDisplay = new SceneDisplayWidget(this);
    setCentralWidget(sceneDisplay);
    resources = JsonResourceFile("./assets/resources.json").getResources(sceneDisplay->getAllocator(), domain);
    scene = JsonSceneFile("./assets/scene.json").loadScene(*resources);
    sceneDisplay->setScene(scene);
    sceneDisplay->setViewer(&viewerPerspective);
}

MainWindow::~MainWindow() {

}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    mana::Mat4f mat = MatrixMath::inverse(MatrixMath::rotate(viewerPerspective.transform.rotation));
    Vec4f tmp = mat * Vec4f(0, 0, -1, 1);
    Vec3f forward(tmp.x, tmp.y, tmp.z);
    tmp = mat * Vec4f(1, 0, 0, 1);
    Vec3f left(tmp.x, tmp.y, tmp.z);
    tmp = mat * Vec4f(0, 1, 0, 1);
    Vec3f up(tmp.x, tmp.y, tmp.z);
    float movspeed = 1;
    float rotspeed = 5;
    switch (event->key()) {
        case Qt::Key_W:
            viewerPerspective.transform.position += forward * movspeed;
            break;
        case Qt::Key_S:
            viewerPerspective.transform.position += forward * -movspeed;
            break;
        case Qt::Key_A:
            viewerPerspective.transform.position += left * -movspeed;
            break;
        case Qt::Key_D:
            viewerPerspective.transform.position += left * movspeed;
            break;
        case Qt::Key_Q:
            viewerPerspective.transform.position += up * movspeed;
            break;
        case Qt::Key_E:
            viewerPerspective.transform.position += up * -movspeed;
            break;
        case Qt::Key_Up:
            viewerPerspective.transform.rotation += Vec3f(1, 0, 0) * rotspeed;
            break;
        case Qt::Key_Down:
            viewerPerspective.transform.rotation += Vec3f(-1, 0, 0) * rotspeed;
            break;
        case Qt::Key_Left:
            viewerPerspective.transform.rotation += Vec3f(0, -1, 0) * rotspeed;
            break;
        case Qt::Key_Right:
            viewerPerspective.transform.rotation += Vec3f(0, 1, 0) * rotspeed;
            break;
    }
    sceneDisplay->setScene(scene);
    sceneDisplay->update();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyReleaseEvent(event);
}


