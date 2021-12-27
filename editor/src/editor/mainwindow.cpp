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

#include "editor/mainwindow.hpp"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMenuBar>
#include <QSizePolicy>

#include "engine.hpp"
#include "common.hpp"

using namespace engine;

MainWindow::MainWindow() {
    menuBar()->addMenu("File");
    rootWidget = new QWidget(this);

    setCentralWidget(rootWidget);

    rootLayout = new QHBoxLayout();

    archive = std::make_unique<DirectoryArchive>(std::filesystem::current_path().string() + "/assets");
    assetManager = std::make_unique<AssetManager>(*archive);

    renderWidget = new RenderWidgetQt(this, *assetManager);
    sceneEditWidget = new SceneEditWidget(this);
    fileBrowser = new FileBrowser(this);

    sceneSplitter = new QSplitter(this);
    fileSplitter = new QSplitter(this);

    fileSplitter->setOrientation(Qt::Vertical);

    fileSplitter->addWidget(sceneEditWidget);
    fileSplitter->addWidget(fileBrowser);

    sceneSplitter->addWidget(renderWidget);
    sceneSplitter->addWidget(fileSplitter);

    rootLayout->addWidget(sceneSplitter);
    rootWidget->setLayout(rootLayout);
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}


