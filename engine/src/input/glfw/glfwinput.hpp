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

#include "engine/input/input.hpp"

namespace engine {
    class GLFWInput : public Input {
    public:
        explicit GLFWInput(GLFWwindow &wndH);

        ~GLFWInput() override;

        void glfwKeyCallback(int key, int scancode, int action, int mods);

        void glfwCursorCallback(double xpos, double ypos);

        void glfwMouseKeyCallback(int button, int action, int mods);

        bool getKeyDown(Key key) override;

        Mouse getMouse() override;

        void registerListener(InputListener &listener) override;

        void unregisterListener(InputListener &listener) override;

        void setClipboardText(std::string text) override;

        std::string getClipboardText() override;

    private:
        GLFWwindow &wndH;

        std::set<InputListener *> listeners;

        Mouse mouse;
        std::map<Key, bool> keys;
    };
}

#endif //MANA_GLFWINPUT_HPP
