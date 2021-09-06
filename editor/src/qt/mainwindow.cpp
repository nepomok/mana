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

#include "engine/io/scenefile.hpp"
#include "engine/io/resourcefile.hpp"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMenuBar>

using namespace engine;

MainWindow::MainWindow() {
    menuBar()->addMenu("File");

    rootWidget = new QWidget(this);

    setCentralWidget(rootWidget);

    rootLayout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    rootLayout->addLayout(leftLayout, 1);
    rootLayout->addLayout(rightLayout, 0);

    sceneDisplay = new SceneDisplayWidget(this);

    leftLayout->addWidget(sceneDisplay);

    rootWidget->setLayout(rootLayout);

    resources = ResourceFile("./assets/resources.json").getResources(sceneDisplay->getDevice(), domain);
    scene = SceneFile("./assets/scene.json").getScene(*resources);

    sceneDisplay->setScene(scene);
    sceneDisplay->setFocusPolicy(Qt::ClickFocus);
}

MainWindow::~MainWindow() = default;

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        if (sceneDisplay->getHighlightedNodes().empty())
            sceneDisplay->setHighlightedNodes({"plane"});
        else
            sceneDisplay->setHighlightedNodes({});
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}


