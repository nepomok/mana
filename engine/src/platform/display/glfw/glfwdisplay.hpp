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


#ifdef BUILD_ENGINE_RENDERER_OPENGL
#include "platform/display/glfw/opengl/glfwwindowgl.hpp"
#endif

#include "glfwmonitor.hpp" //Has to come after glfwwindowgl because of glad include collision with glfw (Including glfw and then glad afterwards gives compiler error, the reverse is legal)

namespace engine {
    namespace glfw {
        std::unique_ptr<Monitor> getPrimaryMonitor() {
            return std::make_unique<GLFWMonitor>(glfwGetPrimaryMonitor());
        }

        std::set<std::unique_ptr<Monitor>> getMonitors() {
            std::set<std::unique_ptr<Monitor>> ret;

            int count;
            GLFWmonitor **monitors = glfwGetMonitors(&count);
            for (int i = 0; i < count; i++) {
                ret.insert(std::make_unique<GLFWMonitor>(monitors[i]));
            }

            return ret;
        }

        std::unique_ptr<Window> createWindow(GraphicsBackend api) {
            switch (api) {
#ifdef BUILD_ENGINE_RENDERER_OPENGL
                case OPENGL_4_6:
                    return std::make_unique<GLFWWindowGL>("Window GLFW", Vec2i(600, 300), WindowAttributes());
#endif
                default:
                    throw std::runtime_error("Unsupported graphics api");
            }
        }

        std::unique_ptr<Window> createWindow(GraphicsBackend api,
                                             const std::string &title,
                                             Vec2i size,
                                             WindowAttributes attributes) {
            switch (api) {
#ifdef BUILD_ENGINE_RENDERER_OPENGL
                case OPENGL_4_6:
                    return std::make_unique<GLFWWindowGL>(title, size, attributes);
#endif
                default:
                    throw std::runtime_error("Unsupported graphics api");
            }
        }

        std::unique_ptr<Window> createWindow(GraphicsBackend api,
                                             const std::string &title,
                                             Vec2i size,
                                             WindowAttributes attributes,
                                             Monitor &monitor,
                                             VideoMode mode) {
            switch (api) {
#ifdef BUILD_ENGINE_RENDERER_OPENGL
                case OPENGL_4_6:
                    return std::make_unique<GLFWWindowGL>(title, size, attributes, dynamic_cast<GLFWMonitor &>(monitor),
                                                          mode);
#endif
                default:
                    throw std::runtime_error("Unsupported graphics api");
            }
        }
    }
}