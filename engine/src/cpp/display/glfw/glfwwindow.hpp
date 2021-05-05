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

#ifndef MANA_GLFWWINDOW_HPP
#define MANA_GLFWWINDOW_HPP

#include <set>

#include "extern/glad.h"

#include "engine/display/window.hpp"
#include "engine/display/windowattributes.hpp"

#include "render/opengl/oglrenderer.hpp"
#include "render/opengl/oglrenderallocator.hpp"
#include "engine/render/opengl/oglframebuffer.hpp"

#include "glfwmonitor.hpp"
#include "glfwwindowframebuffer.hpp"

#include "input/glfw/glfwinput.hpp"

namespace mana {
    namespace glfw {
        class GLFWWindow : public Window {
        public:
            GLFWWindow(const std::string &title, Vec2i size, WindowAttributes attributes);

            GLFWWindow(const std::string &title,
                       Vec2i size,
                       WindowAttributes attributes,
                       GLFWMonitor &monitor);

            ~GLFWWindow() override;

            void glfwWindowCloseCallback();

            void glfwWindowMoveCallback(Vec2i pos);

            void glfwWindowSizeCallback(int width, int height);

            void glfwWindowRefreshCallback();

            void glfwWindowFocusCallback(bool focused);

            void glfwWindowMinimizeCallback();

            void glfwWindowMaximizeCallback();

            void glfwWindowContentScaleCallback(Vec2f scale);

            void glfwFrameBufferSizeCallback(Vec2i size);

            RenderTarget &getRenderTarget() override;

            Input &getInput() override;

            void bind() override;

            void swapBuffers() override;

            void update() override;

            bool shouldClose() override;

            void registerListener(WindowListener &listener) override;

            void unregisterListener(WindowListener &listener) override;

            void maximize() override;

            void minimize() override;

            void restore() override;

            void show() override;

            void hide() override;

            void focus() override;

            void requestAttention() override;

            void setTitle(std::string title) override;

            void setIcon(ImageBuffer<ColorRGBA> &buffer) override;

            void setWindowPosition(Vec2i position) override;

            Vec2i getWindowPosition() override;

            void setWindowSize(Vec2i size) override;

            Vec2i getWindowSize() override;

            void setWindowSizeLimit(Vec2i sizeMin, Vec2i sizeMax) override;

            void setWindowAspectRatio(Vec2i ratio) override;

            Vec2i getFramebufferSize() override;

            Vec4i getFrameSize() override;

            Vec2f getWindowContentScale() override;

            float getWindowOpacity() override;

            void setWindowOpacity(float opacity) override;

            Monitor *getMonitor() override;

            void setMonitor(Monitor *monitor, Recti rect, int refreshRate) override;

            void setWindowed() override;

            void setWindowDecorated(bool decorated) override;

            void setWindowResizable(bool resizable) override;

            void setWindowAlwaysOnTop(bool alwaysOnTop) override;

            void setWindowAutoMinimize(bool autoMinimize) override;

            void setWindowFocusOnShow(bool focusOnShow) override;

        private:
            GLFWwindow *wndH;

            opengl::OGLRenderer *renderer;
            opengl::OGLRenderAllocator *renderAllocator;

            GLFWInput *input;
            GLFWWindowFrameBuffer *frameBuffer;

            std::set<WindowListener *> listeners;
        };
    }
}

#endif //MANA_GLFWWINDOW_HPP
