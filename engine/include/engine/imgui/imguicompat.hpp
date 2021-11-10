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

#ifndef MANA_IMGUICOMPAT_HPP
#define MANA_IMGUICOMPAT_HPP

#include "engine/render/graphicsbackend.hpp"
#include "engine/render/rendertarget.hpp"

#include "engine/display/displaybackend.hpp"

#include "engine/display/window.hpp"

namespace engine {
    namespace ImGuiCompat {
        /**
         * Calls the Impl*Init* methods the user still has to call ImGui::CreateContext beforehand.
         * @param window
         */
        void Init(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend);

        /**
         * Calls the Impl*Shutdown methods, the user still has to call ImGui::DestroyContext afterwards.
         * @param window
         */
        void Shutdown(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend);

        /**
         * Calls Impl*NewFrame methods, the user still has to call ImGui::NewFrame afterwards.
         * @param window
         */
        void NewFrame(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend);

        /**
         * Calls Impl*RenderDrawData and renders the imgui data into the target.
         * The user still has to call ImGui::Render beforehand.
         *
         * @param window
         * @param target
         */
        void DrawData(Window &window, DisplayBackend displayBackend, GraphicsBackend graphicsBackend, RenderTarget &target);
    }
}
#endif //MANA_IMGUICOMPAT_HPP
