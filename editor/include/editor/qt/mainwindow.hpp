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

#ifndef MANA_MAINWINDOW_HPP
#define MANA_MAINWINDOW_HPP

#include <QMainWindow>

#include "editor/qt/widgets/scenedisplaywidget.hpp"

#include "mana.hpp"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private:
    SceneDisplayWidget *sceneDisplay;

    mana::Resources *resources;
    mana::Scene scene;

    mana::MonoCppDomain domain;

    mana::PerspectiveCamera viewerPerspective;
    mana::OrthographicCamera viewerOrthographic;
};

#endif //MANA_MAINWINDOW_HPP
