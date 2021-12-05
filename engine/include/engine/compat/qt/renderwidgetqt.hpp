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

#ifndef MANA_RENDERWIDGETQT_HPP
#define MANA_RENDERWIDGETQT_HPP

#ifdef BUILD_ENGINE_RENDERER_QT

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include "engine/render/api/rendertarget.hpp"
#include "engine/render/deferred/deferredrenderer.hpp"

namespace engine {
    class RenderWidgetQt : public QOpenGLWidget {
    public:
        void setScene(Scene *s) {
            scene = s;
        }

    protected:
        void initializeGL() override {
            QOpenGLWidget::initializeGL();
        }

        void resizeGL(int w, int h) override {
            QOpenGLWidget::resizeGL(w, h);
            ren.getGeometryBuffer().setSize({w, h});
        }

        void paintGL() override {
            QOpenGLWidget::paintGL();
            if (scene != nullptr) {
                //    ren.render()
            }
        }

    private:
        DeferredRenderer ren;
        Scene *scene = nullptr;
    };
}

#endif

#endif //MANA_RENDERWIDGETQT_HPP
