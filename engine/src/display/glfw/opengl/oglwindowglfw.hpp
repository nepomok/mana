#ifndef MANA_OGLWINDOWGLFW_HPP
#define MANA_OGLWINDOWGLFW_HPP

#include <set>

#include "engine/render/window.hpp"
#include "engine/render/windowattributes.hpp"

#include "oglrenderapi.hpp"
#include "oglwindowglfw.hpp"
#include "../monitorglfw.hpp"
#include "oglframebufferobject.hpp"

namespace mana {
    namespace opengl {
        class OGLWindowGLFW : public Window {
        public:
            OGLWindowGLFW(const std::string &title, Vec2i size, WindowAttributes attributes);

            OGLWindowGLFW(std::string title,
                          Vec2i size,
                          WindowAttributes attributes,
                          MonitorGLFW &monitor);

            ~OGLWindowGLFW() override;

            void glfwWindowCloseCallback();

            void glfwWindowMoveCallback(Vec2i pos);

            void glfwWindowSizeCallback(int width, int height);

            void glfwWindowRefreshCallback();

            void glfwWindowFocusCallback(bool focused);

            void glfwWindowMinimizeCallback();

            void glfwWindowMaximizeCallback();

            void glfwWindowContentScaleCallback(Vec2f scale);

            void glfwFrameBufferSizeCallback(Vec2i size);

            RenderAPI &getRenderAPI() override;

            Input &getInput() override;

            FrameBufferObject &getFrameBuffer() override;

            void bindContext() override;

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

            void setIcon(ImageBuffer<ColorRGBA32> &buffer) override;

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

            RenderAPI *renderApi;
            Input *input;
            FrameBufferObject *frameBuffer;

            std::set<WindowListener *> listeners;
        };
    }
}

#endif //MANA_OGLWINDOWGLFW_HPP
