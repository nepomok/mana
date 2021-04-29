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

#include "display/glfw/glfwwindow.hpp"
#include "display/glfw/glfwmonitor.hpp" //Has to come after glfwwindow because of glad include collision with glfw (Including glfw and then glad afterwards gives compiler error, the reverse is legal)

namespace mana {
    namespace glfw {
        Monitor *getPrimaryMonitor() {
            auto *mon = new GLFWMonitor();
            mon->monH = glfwGetPrimaryMonitor();
            return dynamic_cast<Monitor *>(mon);
        }

        std::set<Monitor *> getMonitors() {
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

        Window *createWindow(GraphicsApi api) {
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

        Window *createWindow(GraphicsApi api,
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

        Window *createWindow(GraphicsApi api,
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
}