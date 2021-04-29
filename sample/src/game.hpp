/**
 *  Mana - 3D Engine
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

#ifndef MANA_GAME_HPP
#define MANA_GAME_HPP

#include <chrono>

#include "mana.hpp"

using namespace mana;

class Game : public InputListener, public WindowListener {
public:
    virtual ~Game() = default;

    virtual int loop(GraphicsApi api) {
        DisplayManager dm(mana::GLFW);

        Window *wnd = dm.createWindow(api);

        auto &alloc = wnd->getRenderAllocator();
        auto &ren = wnd->getRenderer();

        ren2d = Renderer2D(ren, alloc);
        ren3d = Renderer3D(ren, alloc);

        Input &input = wnd->getInput();

        start(*wnd, ren, alloc, input);

        float deltaTime = 0;
        auto lastUpdate = std::chrono::high_resolution_clock::now();
        while (!wnd->shouldClose()) {
            auto start = std::chrono::high_resolution_clock::now();
            update(deltaTime, *wnd, ren, alloc, input);
            auto stop = std::chrono::high_resolution_clock::now();
            deltaTime = static_cast<std::chrono::duration<float>>(stop - start).count();
        }

        stop(*wnd, ren, alloc, input);

        return 0;
    }

protected:
    virtual void start(Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) {
        window.registerListener(*this);
        input.registerListener(*this);
        loadScene(alloc);
    }

    /**
     *
     * @param deltaTime The time in seconds which the previous invocation of the update function took.
     * @param window
     * @param ren
     * @param input
     */
    virtual void update(float deltaTime, Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) {
    }

    virtual void stop(Window &window, Renderer &renderApi, RenderAllocator &alloc, Input &input) {
        window.unregisterListener(*this);
        input.unregisterListener(*this);
        destroyScene();
    }

    virtual void loadScene(RenderAllocator &alloc) = 0;

    virtual void destroyScene() = 0;

    Renderer2D ren2d;
    Renderer3D ren3d;
};

#endif //MANA_GAME_HPP
