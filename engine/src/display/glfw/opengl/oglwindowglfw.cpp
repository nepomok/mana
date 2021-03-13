#include "oglwindowglfw.hpp"

#include <stdexcept>

#include "../../../input/glfw/glfwinput.hpp"

#include "oglwindowframebufferobject.hpp"
#include "../monitorglfw.hpp"

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
    namespace opengl {
        std::map<GLFWwindow *, OGLWindowGLFW *> _windowMapping;

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

        OGLWindowGLFW::OGLWindowGLFW(const std::string &title, Vec2i size, WindowAttributes attributes) {
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

            renderApi = dynamic_cast<RenderAPI *>(new OGLRenderAPI());
            input = dynamic_cast<Input *>(new GLFWInput(*wndH));
            frameBuffer = dynamic_cast<FrameBufferObject *>(new OGLWindowFrameBufferObject(wndH));

            _windowMapping[wndH] = this;

            setCallbacks(wndH);
        }

        OGLWindowGLFW::OGLWindowGLFW(std::string title,
                                     Vec2i size,
                                     WindowAttributes attributes,
                                     MonitorGLFW &monitor) {
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

            renderApi = dynamic_cast<RenderAPI *>(new OGLRenderAPI());
            input = dynamic_cast<Input *>(new GLFWInput(*wndH));
            frameBuffer = dynamic_cast<FrameBufferObject *>(new OGLWindowFrameBufferObject(wndH));

            _windowMapping[wndH] = this;

            setCallbacks(wndH);
        }

        OGLWindowGLFW::~OGLWindowGLFW() noexcept {
            _windowMapping.erase(wndH);
            delete dynamic_cast<OGLRenderAPI *>(renderApi);
            delete dynamic_cast<GLFWInput *>(input);
            delete dynamic_cast<OGLWindowFrameBufferObject *>(frameBuffer);
            GLFWCounter::leave();
        }

        void OGLWindowGLFW::glfwWindowCloseCallback() {
            for (auto listener : listeners) {
                listener->onWindowClose();
            }
        }

        void OGLWindowGLFW::glfwWindowMoveCallback(Vec2i pos) {
            for (auto listener : listeners) {
                listener->onWindowMove(pos);
            }
        }

        void OGLWindowGLFW::glfwWindowSizeCallback(int width, int height) {
            for (auto listener : listeners) {
                listener->onWindowResize(Vec2i(width, height));
            }
        }

        void OGLWindowGLFW::glfwWindowRefreshCallback() {
            for (auto listener : listeners) {
                listener->onWindowRefresh();
            }
        }

        void OGLWindowGLFW::glfwWindowFocusCallback(bool focused) {
            for (auto listener : listeners) {
                listener->onWindowFocus(focused);
            }
        }

        void OGLWindowGLFW::glfwWindowMinimizeCallback() {
            for (auto listener : listeners) {
                listener->onWindowMinimize();
            }
        }

        void OGLWindowGLFW::glfwWindowMaximizeCallback() {
            for (auto listener : listeners) {
                listener->onWindowMaximize();
            }
        }

        void OGLWindowGLFW::glfwWindowContentScaleCallback(Vec2f scale) {
            for (auto listener : listeners) {
                listener->onWindowContentScale(scale);
            }
        }

        void OGLWindowGLFW::glfwFrameBufferSizeCallback(Vec2i size) {
            for (auto listener : listeners) {
                listener->onFramebufferResize(size);
            }
        }

        RenderAPI &OGLWindowGLFW::getRenderAPI() {
            return *renderApi;
        }

        Input &OGLWindowGLFW::getInput() {
            return *input;
        }

        FrameBufferObject &OGLWindowGLFW::getFrameBuffer() {
            return *frameBuffer;
        }

        void OGLWindowGLFW::bindContext() {
            glfwMakeContextCurrent(wndH);
        }

        void OGLWindowGLFW::swapBuffers() {
            glfwSwapBuffers(wndH);
        }

        void OGLWindowGLFW::update() {
            glfwPollEvents();
        }

        bool OGLWindowGLFW::shouldClose() {
            return glfwWindowShouldClose(wndH);
        }

        void OGLWindowGLFW::registerListener(WindowListener &listener) {
            if (listeners.find(&listener) != listeners.end())
                throw std::runtime_error("Listener already registered");
            listeners.insert(&listener);
        }

        void OGLWindowGLFW::unregisterListener(WindowListener &listener) {
            listeners.erase(&listener);
        }

        void OGLWindowGLFW::maximize() {
            glfwMaximizeWindow(wndH);
        }

        void OGLWindowGLFW::minimize() {
            glfwIconifyWindow(wndH);
        }

        void OGLWindowGLFW::restore() {
            glfwRestoreWindow(wndH);
        }

        void OGLWindowGLFW::show() {
            glfwShowWindow(wndH);
        }

        void OGLWindowGLFW::hide() {
            glfwHideWindow(wndH);
        }

        void OGLWindowGLFW::focus() {
            glfwFocusWindow(wndH);
        }

        void OGLWindowGLFW::requestAttention() {
            glfwRequestWindowAttention(wndH);
        }

        void OGLWindowGLFW::setTitle(std::string title) {
            glfwSetWindowTitle(wndH, title.c_str());
        }

        void OGLWindowGLFW::setIcon(ImageBuffer<ColorRGBA32> &buffer) {
            throw std::runtime_error("Not Implemented");
        }

        void OGLWindowGLFW::setWindowPosition(Vec2i position) {
            glfwSetWindowPos(wndH, position.x, position.y);
        }

        Vec2i OGLWindowGLFW::getWindowPosition() {
            int x, y;
            glfwGetWindowPos(wndH, &x, &y);
            return {x, y};
        }

        void OGLWindowGLFW::setWindowSize(Vec2i size) {
            glfwSetWindowSize(wndH, size.x, size.y);
        }

        Vec2i OGLWindowGLFW::getWindowSize() {
            int x, y;
            glfwGetWindowSize(wndH, &x, &y);
            return {x, y};
        }

        void OGLWindowGLFW::setWindowSizeLimit(Vec2i sizeMin, Vec2i sizeMax) {
            glfwSetWindowSizeLimits(wndH, sizeMin.x, sizeMin.y, sizeMax.x, sizeMax.y);
        }

        void OGLWindowGLFW::setWindowAspectRatio(Vec2i ratio) {
            glfwSetWindowAspectRatio(wndH, ratio.x, ratio.y);
        }

        Vec2i OGLWindowGLFW::getFramebufferSize() {
            int x, y;
            glfwGetFramebufferSize(wndH, &x, &y);
            return {x, y};
        }

        Vec4i OGLWindowGLFW::getFrameSize() {
            int x, y, z, w;
            glfwGetWindowFrameSize(wndH, &x, &y, &z, &w);
            return {x, y, z, w};
        }

        Vec2f OGLWindowGLFW::getWindowContentScale() {
            float x, y;
            glfwGetWindowContentScale(wndH, &x, &y);
            return {x, y};
        }

        float OGLWindowGLFW::getWindowOpacity() {
            float ret;
            glfwGetWindowOpacity(wndH);
            return ret;
        }

        void OGLWindowGLFW::setWindowOpacity(float opacity) {
            glfwSetWindowOpacity(wndH, opacity);
        }

        Monitor *OGLWindowGLFW::getMonitor() {
            auto *ret = new MonitorGLFW();
            ret->monH = glfwGetWindowMonitor(wndH);
            return ret;
        }

        void OGLWindowGLFW::setMonitor(Monitor *monitor, Recti rect, int refreshRate) {
            auto &mon = dynamic_cast<MonitorGLFW &>(*monitor);
            glfwSetWindowMonitor(wndH,
                                 mon.monH,
                                 rect.position.x,
                                 rect.position.y,
                                 rect.dimensions.x,
                                 rect.dimensions.y,
                                 refreshRate);
        }

        void OGLWindowGLFW::setWindowed() {
            glfwSetWindowMonitor(wndH, NULL, 0, 0, 0, 0, 0);
        }

        void OGLWindowGLFW::setWindowDecorated(bool decorated) {
            glfwSetWindowAttrib(wndH, GLFW_DECORATED, decorated);
        }

        void OGLWindowGLFW::setWindowResizable(bool resizable) {
            glfwSetWindowAttrib(wndH, GLFW_RESIZABLE, resizable);
        }

        void OGLWindowGLFW::setWindowAlwaysOnTop(bool alwaysOnTop) {
            glfwSetWindowAttrib(wndH, GLFW_FLOATING, alwaysOnTop);
        }

        void OGLWindowGLFW::setWindowAutoMinimize(bool autoMinimize) {
            glfwSetWindowAttrib(wndH, GLFW_AUTO_ICONIFY, autoMinimize);
        }

        void OGLWindowGLFW::setWindowFocusOnShow(bool focusOnShow) {
            glfwSetWindowAttrib(wndH, GLFW_FOCUS_ON_SHOW, focusOnShow);
        }
    }
}