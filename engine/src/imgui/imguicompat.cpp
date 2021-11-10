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

#include "engine/imgui/imguicompat.hpp"

#include "engine/display/window.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "display/glfw/opengl/glfwwindowgl.hpp"

namespace engine {
    namespace ImGuiCompat {
        void Init(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend) {
            switch (graphicsBackend) {
                case OPENGL_4_6:
                    ImGui_ImplGlfw_InitForOpenGL(dynamic_cast<glfw::GLFWWindowGL &>(window).wndH, true);
                    ImGui_ImplOpenGL3_Init("#version 460");
                    break;
                case DIRECTX_11:
                case VULKAN:
                    throw std::runtime_error("Not supported");
            }
        }

        void Shutdown(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend) {
            switch (graphicsBackend) {
                case OPENGL_4_6:
                    ImGui_ImplOpenGL3_Shutdown();
                    ImGui_ImplGlfw_Shutdown();
                    break;
                case DIRECTX_11:
                case VULKAN:
                    throw std::runtime_error("Not supported");
            }
        }

        void NewFrame(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend) {
            switch (graphicsBackend) {
                case OPENGL_4_6:
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    break;
                case DIRECTX_11:
                case VULKAN:
                    throw std::runtime_error("Not supported");
            }
        }

        void DrawData(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend, RenderTarget &target) {
            switch (graphicsBackend) {
                case OPENGL_4_6:
                    int display_w, display_h;
                    glViewport(0, 0, window.getFramebufferSize().x, window.getFramebufferSize().y);
                    glClear(0);
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                    break;
                case DIRECTX_11:
                case VULKAN:
                    throw std::runtime_error("Not supported");
            }
        }
    }
}