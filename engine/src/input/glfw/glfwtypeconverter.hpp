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

#ifndef MANA_GLFWTYPECONVERTER_HPP
#define MANA_GLFWTYPECONVERTER_HPP

#include <stdexcept>
#include <GLFW/glfw3.h>

#include "engine/input/key.hpp"

namespace mana {
    class GLFWTypeConverter {
    public:
        static int convertKey(Key key) {
            throw std::runtime_error("Not Implemented");
        }

        static Key convertKey(int key) {
            switch (key) {
                case GLFW_KEY_ENTER:
                    return KEY_RETURN;
                case GLFW_KEY_ESCAPE:
                    return KEY_ESCAPE;
                case GLFW_KEY_BACKSPACE:
                    return KEY_BACKSPACE;
                case GLFW_KEY_TAB:
                    return KEY_TAB;
                case GLFW_KEY_SPACE:
                    return KEY_SPACE;

                case GLFW_KEY_0:
                    return KEY_0;
                case GLFW_KEY_1:
                    return KEY_1;
                case GLFW_KEY_2:
                    return KEY_2;
                case GLFW_KEY_3:
                    return KEY_3;
                case GLFW_KEY_4:
                    return KEY_4;
                case GLFW_KEY_5:
                    return KEY_5;
                case GLFW_KEY_6:
                    return KEY_6;
                case GLFW_KEY_7:
                    return KEY_7;
                case GLFW_KEY_8:
                    return KEY_8;
                case GLFW_KEY_9:
                    return KEY_9;

                case GLFW_KEY_A:
                    return KEY_A;
                case GLFW_KEY_B:
                    return KEY_B;
                case GLFW_KEY_C:
                    return KEY_C;
                case GLFW_KEY_D:
                    return KEY_D;
                case GLFW_KEY_E:
                    return KEY_E;
                case GLFW_KEY_F:
                    return KEY_F;
                case GLFW_KEY_G:
                    return KEY_G;
                case GLFW_KEY_H:
                    return KEY_H;
                case GLFW_KEY_I:
                    return KEY_I;
                case GLFW_KEY_J:
                    return KEY_J;
                case GLFW_KEY_K:
                    return KEY_K;
                case GLFW_KEY_L:
                    return KEY_L;
                case GLFW_KEY_M:
                    return KEY_M;
                case GLFW_KEY_N:
                    return KEY_N;
                case GLFW_KEY_O:
                    return KEY_O;
                case GLFW_KEY_P:
                    return KEY_P;
                case GLFW_KEY_Q:
                    return KEY_Q;
                case GLFW_KEY_R:
                    return KEY_R;
                case GLFW_KEY_S:
                    return KEY_S;
                case GLFW_KEY_T:
                    return KEY_T;
                case GLFW_KEY_U:
                    return KEY_U;
                case GLFW_KEY_V:
                    return KEY_V;
                case GLFW_KEY_W:
                    return KEY_W;
                case GLFW_KEY_X:
                    return KEY_X;
                case GLFW_KEY_Y:
                    return KEY_Y;
                case GLFW_KEY_Z:
                    return KEY_Z;

                case GLFW_KEY_CAPS_LOCK:
                    return KEY_CAPSLOCK;

                case GLFW_KEY_F1:
                    return KEY_F1;
                case GLFW_KEY_F2:
                    return KEY_F2;
                case GLFW_KEY_F3:
                    return KEY_F3;
                case GLFW_KEY_F4:
                    return KEY_F4;
                case GLFW_KEY_F5:
                    return KEY_F5;
                case GLFW_KEY_F6:
                    return KEY_F6;
                case GLFW_KEY_F7:
                    return KEY_F7;
                case GLFW_KEY_F8:
                    return KEY_F8;
                case GLFW_KEY_F9:
                    return KEY_F9;
                case GLFW_KEY_F10:
                    return KEY_F10;
                case GLFW_KEY_F11:
                    return KEY_F11;
                case GLFW_KEY_F12:
                    return KEY_F12;

                case GLFW_KEY_PRINT_SCREEN:
                    return KEY_PRINTSCREEN;
                case GLFW_KEY_SCROLL_LOCK:
                    return KEY_SCROLLLOCK;
                case GLFW_KEY_PAUSE:
                    return KEY_PAUSE;
                case GLFW_KEY_INSERT:
                    return KEY_INSERT;
                case GLFW_KEY_HOME:
                    return KEY_HOME;
                case GLFW_KEY_PAGE_UP:
                    return KEY_PAGEUP;
                case GLFW_KEY_PAGE_DOWN:
                    return KEY_PAGEDOWN;
                case GLFW_KEY_DELETE:
                    return KEY_DELETE;
                case GLFW_KEY_END:
                    return KEY_END;
                case GLFW_KEY_UP:
                    return KEY_UP;
                case GLFW_KEY_DOWN:
                    return KEY_DOWN;
                case GLFW_KEY_LEFT:
                    return KEY_LEFT;
                case GLFW_KEY_RIGHT:
                    return KEY_RIGHT;

                case GLFW_KEY_KP_DIVIDE:
                    return KEY_KP_DIVIDE;
                case GLFW_KEY_KP_MULTIPLY:
                    return KEY_KP_MULTIPLY;
                case GLFW_KEY_KP_SUBTRACT:
                    return KEY_KP_MINUS;
                case GLFW_KEY_KP_ADD:
                    return KEY_KP_PLUS;
                case GLFW_KEY_KP_ENTER:
                    return KEY_KP_ENTER;
                case GLFW_KEY_KP_0:
                    return KEY_KP_0;
                case GLFW_KEY_KP_1:
                    return KEY_KP_1;
                case GLFW_KEY_KP_2:
                    return KEY_KP_2;
                case GLFW_KEY_KP_3:
                    return KEY_KP_3;
                case GLFW_KEY_KP_4:
                    return KEY_KP_4;
                case GLFW_KEY_KP_5:
                    return KEY_KP_5;
                case GLFW_KEY_KP_6:
                    return KEY_KP_6;
                case GLFW_KEY_KP_7:
                    return KEY_KP_7;
                case GLFW_KEY_KP_8:
                    return KEY_KP_8;
                case GLFW_KEY_KP_9:
                    return KEY_KP_9;
                case GLFW_KEY_KP_DECIMAL:
                    return KEY_KP_PERIOD;

                case GLFW_KEY_LEFT_CONTROL:
                    return KEY_LCTRL;
                case GLFW_KEY_LEFT_SHIFT:
                    return KEY_LSHIFT;
                case GLFW_KEY_LEFT_ALT:
                    return KEY_LALT;
                case GLFW_KEY_RIGHT_CONTROL:
                    return KEY_RCTRL;
                case GLFW_KEY_RIGHT_SHIFT:
                    return KEY_RSHIFT;
                case GLFW_KEY_RIGHT_ALT:
                    return KEY_RALT;
            }
            return KEY_UNKNOWN;
        }
    };
}

#endif //MANA_GLFWTYPECONVERTER_HPP
