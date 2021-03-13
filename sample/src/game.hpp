#ifndef MANA_GAME_HPP
#define MANA_GAME_HPP

#include <chrono>

#include "mana.hpp"

using namespace mana;

class Game : public InputListener, public WindowListener {
public:
    virtual int loop(DisplayAPI &displayApi, GraphicsApi api) {
        Window *wnd = displayApi.createWindow(api);
        RenderAPI &renderApi = wnd->getRenderAPI();
        Input &input = wnd->getInput();

        start(*wnd, renderApi, input);

        float deltaTime = 0;
        auto lastUpdate = std::chrono::high_resolution_clock::now();
        while (!wnd->shouldClose()) {
            auto start = std::chrono::high_resolution_clock::now();
            update(deltaTime, *wnd, renderApi, input);
            auto stop = std::chrono::high_resolution_clock::now();
            deltaTime = static_cast<std::chrono::duration<float>>(stop - start).count();
        }

        stop(*wnd, renderApi, input);

        return 0;
    }

protected:
    virtual void start(Window &window, RenderAPI &renderApi, Input &input) {
        window.registerListener(*this);
        input.registerListener(*this);
        loadScene(renderApi);
    }

    /**
     *
     * @param deltaTime The time in seconds which the previous invocation of the update function took.
     * @param window
     * @param renderApi
     * @param input
     */
    virtual void update(float deltaTime, Window &window, RenderAPI &renderApi, Input &input) {
        renderApi.render(scene, window.getFrameBuffer());
        window.swapBuffers();
    }

    virtual void stop(Window &window, RenderAPI &renderApi, Input &input) {
        window.unregisterListener(*this);
        input.unregisterListener(*this);
        destroyScene();
    }

    virtual void loadScene(RenderAPI &renderApi) {}

    virtual void destroyScene() {}

    RenderScene scene;
};

#endif //MANA_GAME_HPP
