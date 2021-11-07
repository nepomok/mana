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

#ifndef MANA_GLFWINPUT_HPP
#define MANA_GLFWINPUT_HPP

#include <set>
#include <map>

#include <GLFW/glfw3.h>
#include <engine/asset/image.hpp>

#include "engine/input/input.hpp"

namespace engine {
    class GLFWInput : public Input {
    public:
        explicit GLFWInput(GLFWwindow &wndH);

        ~GLFWInput() override;

        void glfwKeyCallback(int key, int scancode, int action, int mods);

        void glfwCursorCallback(double xpos, double ypos);

        void glfwMouseKeyCallback(int button, int action, int mods);

        void glfwJoystickCallback(int jid, int event);

        void addListener(InputListener &listener) override;

        void removeListener(InputListener &listener) override;

        void setClipboardText(std::string text) override;

        std::string getClipboardText() override;

        void setMouseCursorImage(const Image <ColorRGBA> &image) override;

        void clearMouseCursorImage() override;

        bool getKey(keyboard::Key key) override;

        bool getMouseButton(mouse::Button key) override;

        Vec2d getMousePosition() override;

        std::set<int> getGamepads() override;

        std::string getGamepadName(int id) override;

        float getGamepadAxis(int id, gamepad::Axis axis) override;

        bool getGamepadButton(int id, gamepad::Button key) override;

    private:
        GLFWwindow &wndH;

        std::set<InputListener *> listeners;
        std::set<int> gamepads;
    };
}

#endif //MANA_GLFWINPUT_HPP
