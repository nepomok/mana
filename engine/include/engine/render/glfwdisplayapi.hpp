#ifndef MANA_GLFWDISPLAYAPI_HPP
#define MANA_GLFWDISPLAYAPI_HPP

#include "engine/render/displayapi.hpp"

namespace mana {
    class GLFWDisplayAPI : public DisplayAPI {
    public:
        GLFWDisplayAPI() = default;

        ~GLFWDisplayAPI() override = default;

        Monitor *getPrimaryMonitor() override;

        std::set<Monitor *> getMonitors() override;

        Window *createWindow(GraphicsApi api) override;

        Window *createWindow(GraphicsApi api, std::string title, Vec2i size, WindowAttributes attributes) override;

        Window *createWindow(GraphicsApi api,
                             std::string title,
                             Vec2i size,
                             WindowAttributes attributes,
                             Monitor &monitor,
                             VideoMode mode) override;
    };
}

#endif //MANA_GLFWDISPLAYAPI_HPP
