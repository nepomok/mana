#include "engine/display/displayapi.hpp"

#include "display/glfw/glfwwindow.hpp"
#include "display/glfw/glfwmonitor.hpp" //Has to come after glfwwindow because of glad include collision with glfw (Including glfw and then glad afterwards gives compiler error, the reverse is legal)

namespace mana {
    Monitor *DisplayAPI::getPrimaryMonitor() {
        auto *mon = new GLFWMonitor();
        mon->monH = glfwGetPrimaryMonitor();
        return dynamic_cast<Monitor *>(mon);
    }

    std::set<Monitor *> DisplayAPI::getMonitors() {
        std::set<Monitor *> ret;

        int count;
        GLFWmonitor **monitors = glfwGetMonitors(&count);
        for (int i = 0; i < count; i++) {
            auto *mon = new GLFWMonitor();
            mon->monH = monitors[i];
            ret.insert(dynamic_cast<Monitor *>(mon));
        }

        return ret;
    }

    Window *DisplayAPI::createWindow(GraphicsApi api) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new GLFWWindow("Window GLFW", Vec2i(600, 300),
                                                            WindowAttributes()));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }

    Window *DisplayAPI::createWindow(GraphicsApi api,
                                         std::string title,
                                         Vec2i size,
                                         WindowAttributes attributes) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new GLFWWindow(title, size, attributes));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }

    Window *DisplayAPI::createWindow(GraphicsApi api,
                                         std::string title,
                                         Vec2i size,
                                         WindowAttributes attributes,
                                         Monitor &monitor,
                                         VideoMode mode) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new GLFWWindow(title,
                                                            size,
                                                            attributes,
                                                            dynamic_cast<GLFWMonitor &>(monitor)));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }
}