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

#include "engine/input/key.hpp"
#include "engine/input/mousekey.hpp"

namespace engine {
    class InputListener {
    public:
        virtual void onKeyDown(Key key) {};

        virtual void onKeyUp(Key key) {};

        virtual void onMouseMove(double xPos, double yPos) {};

        virtual void onMouseWheelScroll(double amount) {};

        virtual void onMouseKeyDown(MouseKey key) {};

        virtual void onMouseKeyUp(MouseKey key) {};

        virtual void onTextInput(const std::string &text) {};

        //Todo: Gamepad support
    };
}

#endif //MANA_INPUTLISTENER_HPP
