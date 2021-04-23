#ifndef MANA_GAME_HPP
#define MANA_GAME_HPP

#include <chrono>

#include "mana.hpp"

using namespace mana;

class Game : public InputListener, public WindowListener {
public:
    virtual int loop(GraphicsApi api) {
        Window *wnd = DisplayAPI::createWindow(api);
        Renderer &ren = wnd->getRenderer();
        RenderAllocator &alloc = wnd->getRenderAllocator();
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
        ren.render(window.getFrameBuffer(), scene);
        window.swapBuffers();
    }

    virtual void stop(Window &window, Renderer &renderApi, RenderAllocator &alloc, Input &input) {
        window.unregisterListener(*this);
        input.unregisterListener(*this);
        destroyScene();
    }

    virtual void loadScene(RenderAllocator &alloc) = 0;

    virtual void destroyScene() = 0;

    RenderScene scene;
};

#endif //MANA_GAME_HPP
