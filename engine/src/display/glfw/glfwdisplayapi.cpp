#include "engine/render/glfwdisplayapi.hpp"

#include "monitorglfw.hpp"
#include "opengl/oglwindowglfw.hpp"

namespace mana {
    Monitor *GLFWDisplayAPI::getPrimaryMonitor() {
        auto *mon = new MonitorGLFW();
        mon->monH = glfwGetPrimaryMonitor();
        return dynamic_cast<Monitor *>(mon);
    }

    std::set<Monitor *> GLFWDisplayAPI::getMonitors() {
        std::set<Monitor *> ret;

        int count;
        GLFWmonitor **monitors = glfwGetMonitors(&count);
        for (int i = 0; i < count; i++) {
            auto *mon = new MonitorGLFW();
            mon->monH = monitors[i];
            ret.insert(dynamic_cast<Monitor *>(mon));
        }

        return ret;
    }

    Window *GLFWDisplayAPI::createWindow(GraphicsApi api) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new opengl::OGLWindowGLFW("Window OpenGL", Vec2i(600, 300),
                                                                       WindowAttributes()));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }

    Window *GLFWDisplayAPI::createWindow(GraphicsApi api,
                                         std::string title,
                                         Vec2i size,
                                         WindowAttributes attributes) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new opengl::OGLWindowGLFW(title, size, attributes));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }

    Window *GLFWDisplayAPI::createWindow(GraphicsApi api,
                                         std::string title,
                                         Vec2i size,
                                         WindowAttributes attributes,
                                         Monitor &monitor,
                                         VideoMode mode) {
        Window *ret;
        switch (api) {
            case OPENGL:
                ret = dynamic_cast<Window *>(new opengl::OGLWindowGLFW(title,
                                                                       size,
                                                                       attributes,
                                                                       dynamic_cast<MonitorGLFW &>(monitor)));
                break;
            default:
                throw std::runtime_error("Unsupported graphics api");
        }
        return ret;
    }
}