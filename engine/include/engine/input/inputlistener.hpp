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

#ifndef MANA_INPUTLISTENER_HPP
#define MANA_INPUTLISTENER_HPP

#include <string>

#include "engine/input/device/gamepad.hpp"
#include "engine/input/device/keyboard.hpp"
#include "engine/input/device/mouse.hpp"

namespace engine {
    class InputListener {
    public:
        virtual void onKeyDown(keyboard::Key key) {};

        virtual void onKeyUp(keyboard::Key key) {};

        /**
         * @param xPos The mouse x position in pixels relative to the window.
         * @param yPos The mouse y position in pixels relative to the window.
         */
        virtual void onMouseMove(double xPos, double yPos) {};

        /**
         * @param amount The scroll delta
         */
        virtual void onMouseWheelScroll(double amount) {};

        virtual void onMouseKeyDown(mouse::Button key) {};

        virtual void onMouseKeyUp(mouse::Button key) {};

        virtual void onTextInput(const std::string &text) {};

        virtual void onGamepadConnected(int id) {};

        virtual void onGamepadDisconnected(int id) {};

        virtual void onGamepadAxis(int id, gamepad::Axis axis, double amount) {};

        virtual void onGamepadButtonDown(int id, gamepad::Button button) {};

        virtual void onGamepadButtonUp(int id, gamepad::Button button) {};
    };
}

#endif //MANA_INPUTLISTENER_HPP
