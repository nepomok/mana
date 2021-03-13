#ifndef MANA_WINDOW_HPP
#define MANA_WINDOW_HPP

#include "engine/render/framebufferobject.hpp"
#include "engine/render/renderapi.hpp"
#include "engine/display/windowlistener.hpp"
#include "engine/display/monitor.hpp"
#include "engine/io/imagebuffer.hpp"
#include "engine/input/input.hpp"
#include "engine/math/vector2.hpp"

namespace mana {
    /**
     * TODO:Design: When using multiple windows before destroying allocated render objects the corresponding window context has to be bound.
     */
    class Window {
    public:
        virtual ~Window() = default;

        virtual RenderAPI &getRenderAPI() = 0;

        virtual Input &getInput() = 0;

        virtual FrameBufferObject &getFrameBuffer() = 0;

        virtual void bindContext() = 0;

        virtual void swapBuffers() = 0;

        virtual void update() = 0;

        virtual bool shouldClose() = 0;

        virtual void registerListener(WindowListener &listener) = 0;

        virtual void unregisterListener(WindowListener &listener) = 0;

        virtual void maximize() = 0;

        virtual void minimize() = 0;

        virtual void restore() = 0;

        virtual void show() = 0;

        virtual void hide() = 0;

        virtual void focus() = 0;

        virtual void requestAttention() = 0;

        virtual void setTitle(std::string title) = 0;

        virtual void setIcon(ImageBuffer<ColorRGBA32> &buffer) = 0;

        virtual void setWindowPosition(Vec2i position) = 0;

        virtual Vec2i getWindowPosition() = 0;

        virtual void setWindowSize(Vec2i size) = 0;

        virtual Vec2i getWindowSize() = 0;

        virtual void setWindowSizeLimit(Vec2i sizeMin, Vec2i sizeMax) = 0;

        virtual void setWindowAspectRatio(Vec2i ratio) = 0;

        virtual Vec2i getFramebufferSize() = 0;

        virtual Vec4i getFrameSize() = 0;

        virtual Vec2f getWindowContentScale() = 0;

        virtual float getWindowOpacity() = 0;

        virtual void setWindowOpacity(float opacity) = 0;

        virtual Monitor *getMonitor() = 0;

        virtual void setMonitor(Monitor *monitor, Recti rect, int refreshRate) = 0;

        virtual void setWindowed() = 0;

        virtual void setWindowDecorated(bool decorated) = 0;

        virtual void setWindowResizable(bool resizable) = 0;

        virtual void setWindowAlwaysOnTop(bool alwaysOnTop) = 0;

        virtual void setWindowAutoMinimize(bool autoMinimize) = 0;

        virtual void setWindowFocusOnShow(bool focusOnShow) = 0;
    };
}

#endif //MANA_WINDOW_HPP
