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

#ifndef MANA_IMGUIPASS_HPP
#define MANA_IMGUIPASS_HPP

#include <functional>

#include "engine/render/3d/renderpass.hpp"

#include "engine/display/window.hpp"

namespace engine {
    class ImGuiPass : public RenderPass {
    public:
        /**
         * A widget can call all imgui functions except setup, rendering and cleanup functions,
         * which are handled by the pass.
         */
        class Widget {
        public:
            virtual void draw(Scene &scene) = 0;
        };

        explicit ImGuiPass(Window &window, std::vector<std::reference_wrapper<Widget>> commands = {});

        ~ImGuiPass() override;

        void prepareBuffer(GeometryBuffer &gBuffer) override;

        void render(GeometryBuffer &gBuffer, Scene &scene) override;

        void setWidgets(const std::vector<std::reference_wrapper<Widget>> &widgets);

    private:
        Window &window;
        std::vector<std::reference_wrapper<Widget>> widgets;
    };
}

#endif //MANA_IMGUIPASS_HPP
