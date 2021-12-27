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
#include "base64.hpp"

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

    middleSplitter = new QSplitter(this);
    leftSplitter = new QSplitter(this);
    rightSplitter = new QSplitter(this);

    tabWidget = new QTabWidget(this);

    leftSplitter->setOrientation(Qt::Vertical);
    rightSplitter->setOrientation(Qt::Vertical);

    leftSplitter->addWidget(renderWidget);
    leftSplitter->addWidget(tabWidget);

    rightSplitter->addWidget(sceneEditWidget);
    rightSplitter->addWidget(fileBrowser);

    middleSplitter->addWidget(leftSplitter);
    middleSplitter->addWidget(rightSplitter);

    rootLayout->addWidget(middleSplitter);

    rootWidget->setLayout(rootLayout);

    renderScene.skybox.texture = {"/textures/skybox_sky.json", ""};

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    renderWidget->setScene(renderScene);

    timer.start(1000 / 60);

    loadStateFile();
}

MainWindow::~MainWindow() {
    saveStateFile();
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

void MainWindow::onTimeout() {
    renderWidget->setScene(renderScene);
}

void MainWindow::loadStateFile() {
    try {
        std::ifstream fs("mana_editor_state.json");
        if (fs.good()) {
            JsonProtocol jsonProtocol;
            auto msg = jsonProtocol.deserialize(fs);

            std::string dec;
            Base64::Decode(msg.at("leftSplitter").getString(), dec);
            leftSplitter->restoreState(QByteArray::fromStdString(dec));
            Base64::Decode(msg.at("rightSplitter").getString(), dec);
            rightSplitter->restoreState(QByteArray::fromStdString(dec));
            Base64::Decode(msg.at("middleSplitter").getString(), dec);
            middleSplitter->restoreState(QByteArray::fromStdString(dec));
            Base64::Decode(msg.at("sceneEditSplitter").getString(), dec);
            sceneEditWidget->splitter->restoreState(QByteArray::fromStdString(dec));
        }
    } catch (const std::exception &e) {}
}

void MainWindow::saveStateFile() {
    Message msg((std::map<std::string, Message>()));
    msg["leftSplitter"] = Base64::Encode(leftSplitter->saveState().toStdString());
    msg["rightSplitter"] = Base64::Encode(rightSplitter->saveState().toStdString());
    msg["middleSplitter"] = Base64::Encode(middleSplitter->saveState().toStdString());
    msg["sceneEditSplitter"] = Base64::Encode(sceneEditWidget->splitter->saveState().toStdString());

    std::ofstream fs("mana_editor_state.json");
    JsonProtocol jsonProtocol;
    jsonProtocol.serialize(fs, msg);
}


