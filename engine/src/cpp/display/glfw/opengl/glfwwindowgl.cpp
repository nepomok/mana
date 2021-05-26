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

#include <stdexcept>

#include "glfwwindowgl.hpp"

class GLFWCounter {
public:
    static void join() {
        if (counter == 0)
            glfwInit();
        counter++;
    }

    static void leave() {
        counter--;
        if (counter == 0)
            glfwTerminate();
    }

    static int counter;
};

int GLFWCounter::counter = 0;

namespace mana {
    namespace glfw {
        std::map<GLFWwindow *, GLFWWindowGL *> _windowMapping;

        void glfwWindowCloseCallback(GLFWwindow *window) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowCloseCallback();
            } else {
                fprintf(stderr, "Received glfw resize with non registered window.");
            }
        }

        void glfwWindowPositionCallback(GLFWwindow *window, int posx, int posy) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowMoveCallback(Vec2i(posx, posy));
            } else {
                fprintf(stderr, "Received glfw resize with non registered window.");
            }
        }

        void glfwWindowSizeCallback(GLFWwindow *window, int width, int height) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowSizeCallback(width, height);
            } else {
                fprintf(stderr, "Received glfw resize with non registered window.");
            }
        }

        void glfwWindowRefreshCallback(GLFWwindow *window) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowRefreshCallback();
            } else {
                fprintf(stderr, "Received glfw refresh with non registered window.");
            }
        }

        void glfwWindowFocusCallback(GLFWwindow *window, int focus) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowFocusCallback(focus == GLFW_TRUE);
            } else {
                fprintf(stderr, "Received glfw resizeCallback with non registered window.");
            }
        }

        void glfwWindowMinimizeCallback(GLFWwindow *window, int minimized) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowMinimizeCallback();
            } else {
                fprintf(stderr, "Received glfw resizeCallback with non registered window.");
            }
        }

        void glfwWindowMaximizeCallback(GLFWwindow *window, int maximized) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowMaximizeCallback();
            } else {
                fprintf(stderr, "Received glfw resizeCallback with non registered window.");
            }
        }

        void glfwWindowContentScaleCallback(GLFWwindow *window, float scaleX, float scaleY) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwWindowContentScaleCallback(Vec2f(scaleX, scaleY));
            } else {
                fprintf(stderr, "Received glfw resizeCallback with non registered window.");
            }
        }

        void glfwFrameBufferSizeCallback(GLFWwindow *window, int sizeX, int sizeY) {
            if (_windowMapping.find(window) != _windowMapping.end()) {
                _windowMapping[window]->glfwFrameBufferSizeCallback(Vec2i(sizeX, sizeY));
            } else {
                fprintf(stderr, "Received glfw resizeCallback with non registered window.");
            }
        }

        void applyHints(WindowAttributes attribs) {
            glfwWindowHint(GLFW_RESIZABLE, attribs.resizable);
            glfwWindowHint(GLFW_VISIBLE, attribs.visible);
            glfwWindowHint(GLFW_DECORATED, attribs.decorated);
            glfwWindowHint(GLFW_FOCUSED, attribs.focused);
            glfwWindowHint(GLFW_AUTO_ICONIFY, attribs.autoMinimize);
            glfwWindowHint(GLFW_FLOATING, attribs.alwaysOnTop);
            glfwWindowHint(GLFW_MAXIMIZED, attribs.maximized);
            glfwWindowHint(GLFW_CENTER_CURSOR, attribs.centerCursor);
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, attribs.transparentFramebuffer);
            glfwWindowHint(GLFW_FOCUS_ON_SHOW, attribs.focusOnShow);
            glfwWindowHint(GLFW_SCALE_TO_MONITOR, attribs.scaleToMonitor);
            glfwWindowHint(GLFW_RED_BITS, attribs.bitsRed);
            glfwWindowHint(GLFW_GREEN_BITS, attribs.bitsGreen);
            glfwWindowHint(GLFW_BLUE_BITS, attribs.bitsBlue);
            glfwWindowHint(GLFW_ALPHA_BITS, attribs.bitsAlpha);
            glfwWindowHint(GLFW_DEPTH_BITS, attribs.bitsDepth);
            glfwWindowHint(GLFW_STENCIL_BITS, attribs.bitsStencil);
            glfwWindowHint(GLFW_SAMPLES, attribs.msaaSamples);
            glfwWindowHint(GLFW_SRGB_CAPABLE, attribs.sRGBCapable);
            glfwWindowHint(GLFW_DOUBLEBUFFER, attribs.doubleBuffer);
        }

        void setCallbacks(GLFWwindow *wndH) {
            glfwSetWindowCloseCallback(wndH, glfwWindowCloseCallback);
            glfwSetWindowPosCallback(wndH, glfwWindowPositionCallback);
            glfwSetWindowSizeCallback(wndH, glfwWindowSizeCallback);
            glfwSetWindowRefreshCallback(wndH, glfwWindowRefreshCallback);
            glfwSetWindowFocusCallback(wndH, glfwWindowFocusCallback);
            glfwSetWindowIconifyCallback(wndH, glfwWindowMinimizeCallback);
            glfwSetWindowMaximizeCallback(wndH, glfwWindowMaximizeCallback);
            glfwSetWindowContentScaleCallback(wndH, glfwWindowContentScaleCallback);
            glfwSetFramebufferSizeCallback(wndH, glfwFrameBufferSizeCallback);
        }

        GLFWWindowGL::GLFWWindowGL(const std::string &title, Vec2i size, WindowAttributes attributes) {
            GLFWCounter::join();

            glfwDefaultWindowHints();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            applyHints(attributes);

            wndH = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
            if (wndH == NULL) {
                GLFWCounter::leave();
                throw std::runtime_error("Failed to create GLFW Window");
            }

            glfwMakeContextCurrent(wndH);
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                GLFWCounter::leave();
                throw std::runtime_error("Failed to initialize glad");
            }

            renderDevice = new opengl::OGLRenderDevice();
            renderTarget = new GLFWRenderTargetGL(wndH);

            input = new GLFWInput(*wndH);

            _windowMapping[wndH] = this;

            setCallbacks(wndH);
        }

        GLFWWindowGL::GLFWWindowGL(const std::string &title,
                                   Vec2i size,
                                   WindowAttributes attributes,
                                   GLFWMonitor &monitor) {
            GLFWCounter::join();

            glfwDefaultWindowHints();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            applyHints(attributes);

            wndH = glfwCreateWindow(size.x, size.y, title.c_str(), monitor.monH, NULL);
            if (wndH == NULL) {
                GLFWCounter::leave();
                throw std::runtime_error("Failed to create GLFW Window");
            }

            glfwMakeContextCurrent(wndH);
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                GLFWCounter::leave();
                throw std::runtime_error("Failed to initialize glad");
            }

            renderDevice = new opengl::OGLRenderDevice();
            renderTarget = new GLFWRenderTargetGL(wndH);

            input = new GLFWInput(*wndH);

            _windowMapping[wndH] = this;

            setCallbacks(wndH);
        }

        GLFWWindowGL::~GLFWWindowGL() {
            _windowMapping.erase(wndH);
            delete renderTarget;
            delete renderDevice;
            delete input;
            GLFWCounter::leave();
        }

        void GLFWWindowGL::glfwWindowCloseCallback() {
            for (auto listener : listeners) {
                listener->onWindowClose();
            }
        }

        void GLFWWindowGL::glfwWindowMoveCallback(Vec2i pos) {
            for (auto listener : listeners) {
                listener->onWindowMove(pos);
            }
        }

        void GLFWWindowGL::glfwWindowSizeCallback(int width, int height) {
            for (auto listener : listeners) {
                listener->onWindowResize(Vec2i(width, height));
            }
        }

        void GLFWWindowGL::glfwWindowRefreshCallback() {
            for (auto listener : listeners) {
                listener->onWindowRefresh();
            }
        }

        void GLFWWindowGL::glfwWindowFocusCallback(bool focused) {
            for (auto listener : listeners) {
                listener->onWindowFocus(focused);
            }
        }

        void GLFWWindowGL::glfwWindowMinimizeCallback() {
            for (auto listener : listeners) {
                listener->onWindowMinimize();
            }
        }

        void GLFWWindowGL::glfwWindowMaximizeCallback() {
            for (auto listener : listeners) {
                listener->onWindowMaximize();
            }
        }

        void GLFWWindowGL::glfwWindowContentScaleCallback(Vec2f scale) {
            for (auto listener : listeners) {
                listener->onWindowContentScale(scale);
            }
        }

        void GLFWWindowGL::glfwFrameBufferSizeCallback(Vec2i size) {
            for (auto listener : listeners) {
                listener->onFramebufferResize(size);
            }
        }

        RenderDevice &GLFWWindowGL::getRenderDevice() {
            return dynamic_cast<RenderDevice &>(*renderDevice);
        }

        RenderTarget &GLFWWindowGL::getRenderTarget() {
            return dynamic_cast<RenderTarget &>(*renderTarget);
        }

        Input &GLFWWindowGL::getInput() {
            return dynamic_cast<Input &>(*input);
        }

        void GLFWWindowGL::makeCurrent() {
            glfwMakeContextCurrent(wndH);
        }

        void GLFWWindowGL::swapBuffers() {
            glfwSwapBuffers(wndH);
        }

        void GLFWWindowGL::update() {
            glfwPollEvents();
        }

        bool GLFWWindowGL::shouldClose() {
            return glfwWindowShouldClose(wndH);
        }

        void GLFWWindowGL::registerListener(WindowListener &listener) {
            if (listeners.find(&listener) != listeners.end())
                throw std::runtime_error("Listener already registered");
            listeners.insert(&listener);
        }

        void GLFWWindowGL::unregisterListener(WindowListener &listener) {
            listeners.erase(&listener);
        }

        void GLFWWindowGL::maximize() {
            glfwMaximizeWindow(wndH);
        }

        void GLFWWindowGL::minimize() {
            glfwIconifyWindow(wndH);
        }

        void GLFWWindowGL::restore() {
            glfwRestoreWindow(wndH);
        }

        void GLFWWindowGL::show() {
            glfwShowWindow(wndH);
        }

        void GLFWWindowGL::hide() {
            glfwHideWindow(wndH);
        }

        void GLFWWindowGL::focus() {
            glfwFocusWindow(wndH);
        }

        void GLFWWindowGL::requestAttention() {
            glfwRequestWindowAttention(wndH);
        }

        void GLFWWindowGL::setTitle(std::string title) {
            glfwSetWindowTitle(wndH, title.c_str());
        }

        void GLFWWindowGL::setIcon(ImageBuffer<ColorRGBA> &buffer) {
            throw std::runtime_error("Not Implemented");
        }

        void GLFWWindowGL::setWindowPosition(Vec2i position) {
            glfwSetWindowPos(wndH, position.x, position.y);
        }

        Vec2i GLFWWindowGL::getWindowPosition() {
            int x, y;
            glfwGetWindowPos(wndH, &x, &y);
            return {x, y};
        }

        void GLFWWindowGL::setWindowSize(Vec2i size) {
            glfwSetWindowSize(wndH, size.x, size.y);
        }

        Vec2i GLFWWindowGL::getWindowSize() {
            int x, y;
            glfwGetWindowSize(wndH, &x, &y);
            return {x, y};
        }

        void GLFWWindowGL::setWindowSizeLimit(Vec2i sizeMin, Vec2i sizeMax) {
            glfwSetWindowSizeLimits(wndH, sizeMin.x, sizeMin.y, sizeMax.x, sizeMax.y);
        }

        void GLFWWindowGL::setWindowAspectRatio(Vec2i ratio) {
            glfwSetWindowAspectRatio(wndH, ratio.x, ratio.y);
        }

        Vec2i GLFWWindowGL::getFramebufferSize() {
            int x, y;
            glfwGetFramebufferSize(wndH, &x, &y);
            return {x, y};
        }

        Vec4i GLFWWindowGL::getFrameSize() {
            int x, y, z, w;
            glfwGetWindowFrameSize(wndH, &x, &y, &z, &w);
            return {x, y, z, w};
        }

        Vec2f GLFWWindowGL::getWindowContentScale() {
            float x, y;
            glfwGetWindowContentScale(wndH, &x, &y);
            return {x, y};
        }

        float GLFWWindowGL::getWindowOpacity() {
            float ret = glfwGetWindowOpacity(wndH);
            return ret;
        }

        void GLFWWindowGL::setWindowOpacity(float opacity) {
            glfwSetWindowOpacity(wndH, opacity);
        }

        Monitor *GLFWWindowGL::getMonitor() {
            auto *ret = new GLFWMonitor();
            ret->monH = glfwGetWindowMonitor(wndH);
            return ret;
        }

        void GLFWWindowGL::setMonitor(Monitor *monitor, Recti rect, int refreshRate) {
            auto &mon = dynamic_cast<GLFWMonitor &>(*monitor);
            glfwSetWindowMonitor(wndH,
                                 mon.monH,
                                 rect.position.x,
                                 rect.position.y,
                                 rect.dimensions.x,
                                 rect.dimensions.y,
                                 refreshRate);
        }

        void GLFWWindowGL::setWindowed() {
            glfwSetWindowMonitor(wndH, NULL, 0, 0, 0, 0, 0);
        }

        void GLFWWindowGL::setWindowDecorated(bool decorated) {
            glfwSetWindowAttrib(wndH, GLFW_DECORATED, decorated);
        }

        void GLFWWindowGL::setWindowResizable(bool resizable) {
            glfwSetWindowAttrib(wndH, GLFW_RESIZABLE, resizable);
        }

        void GLFWWindowGL::setWindowAlwaysOnTop(bool alwaysOnTop) {
            glfwSetWindowAttrib(wndH, GLFW_FLOATING, alwaysOnTop);
        }

        void GLFWWindowGL::setWindowAutoMinimize(bool autoMinimize) {
            glfwSetWindowAttrib(wndH, GLFW_AUTO_ICONIFY, autoMinimize);
        }

        void GLFWWindowGL::setWindowFocusOnShow(bool focusOnShow) {
            glfwSetWindowAttrib(wndH, GLFW_FOCUS_ON_SHOW, focusOnShow);
        }
    }
}