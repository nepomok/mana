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

#ifndef MANA_WINDOW_HPP
#define MANA_WINDOW_HPP

#include "engine/render/rendertarget.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/renderallocator.hpp"
#include "engine/display/windowlistener.hpp"
#include "engine/display/monitor.hpp"
#include "engine/render/imagebuffer.hpp"
#include "engine/input/input.hpp"
#include "engine/math/vector2.hpp"
#include "engine/display/windowattributes.hpp"
#include "engine/render/graphicsapi.hpp"

namespace mana {
    class Window {
    public:
        const GraphicsApi graphicsApi;

        explicit Window(GraphicsApi graphicsApi) : graphicsApi(graphicsApi) {}

        virtual ~Window() = default;

        /**
         * This method returns the window framebuffer render target.
         * When rendering to the returned buffer the results are displayed in the window.
         *
         * @return
         */
        virtual RenderTarget &getRenderTarget() = 0;

        virtual Input &getInput() = 0;

        /**
         * Calling this method ensures that further rendering calls will be forwarded to this windows context.
         * If using only a single window this method  can be ignored.
         * (On opengl bind window context)
         */
        virtual void bind() = 0;

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

        virtual void setIcon(ImageBuffer<ColorRGBA> &buffer) = 0;

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
