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

#ifdef BUILD_ENGINE_DISPLAY_GLFW

#include <map>
#include <stdexcept>
#include <mutex>

#include "glfwinput.hpp"

#include "glfwtypeconverter.hpp"

namespace engine {
    std::mutex windowMappingMutex;
    std::map<GLFWwindow *, GLFWInput *> windowMapping;

    void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwKeyCallback(key, scancode, action, mods);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    void glfwCursorCallback(GLFWwindow *window, double xpos, double ypos) {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwCursorCallback(xpos, ypos);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    void glfwMouseKeyCallback(GLFWwindow *window, int button, int action, int mods) {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwMouseKeyCallback(button, action, mods);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    void glfwJoystickCallback(int jid, int event) {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        for (auto &pair: windowMapping) {
            pair.second->glfwJoystickCallback(jid, event);
        }
    }

    GLFWInput::GLFWInput(GLFWwindow &wndH) : wndH(wndH) {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        windowMapping[&wndH] = this;

        glfwSetKeyCallback(&wndH, engine::glfwKeyCallback);
        glfwSetCursorPosCallback(&wndH, engine::glfwCursorCallback);
        glfwSetMouseButtonCallback(&wndH, engine::glfwMouseKeyCallback);

        //GLFW Does not appear to send connected events for joysticks which are already connected when the application starts.
        for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_16; i++) {
            if (glfwJoystickIsGamepad(i)) {
                gamepads.insert(i);
                for (auto listener: listeners)
                    listener->onGamepadConnected(i);
            }
        }

        glfwSetJoystickCallback(engine::glfwJoystickCallback);
    }

    GLFWInput::~GLFWInput() {
        std::lock_guard<std::mutex> guard(windowMappingMutex);
        windowMapping.erase(&wndH);
    }

    void GLFWInput::glfwKeyCallback(int key, int scancode, int action, int mods) {
        keyboard::Key k = GLFWTypeConverter::convertKey(key);
        bool kd = action != GLFW_RELEASE;
        for (auto listener: listeners) {
            if (kd)
                listener->onKeyDown(k);
            else
                listener->onKeyUp(k);
        }
    }

    void GLFWInput::glfwCursorCallback(double xpos, double ypos) {
        for (auto listener: listeners) {
            listener->onMouseMove(xpos, ypos);
        }
    }

    void GLFWInput::glfwMouseKeyCallback(int button, int action, int mods) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                for (auto listener: listeners) {
                    listener->onMouseKeyDown(mouse::LEFT);
                }
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                for (auto listener: listeners) {
                    listener->onMouseKeyDown(mouse::MIDDLE);
                }
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                for (auto listener: listeners) {
                    listener->onMouseKeyDown(mouse::RIGHT);
                }
                break;
        }
    }

    void GLFWInput::glfwJoystickCallback(int jid, int event) {
        if (glfwJoystickIsGamepad(jid)) {
            switch (event) {
                case GLFW_CONNECTED:
                    gamepads.insert(jid);
                    for (auto listener: listeners)
                        listener->onGamepadConnected(jid);
                    break;
                case GLFW_DISCONNECTED:
                    gamepads.erase(jid);
                    for (auto listener: listeners)
                        listener->onGamepadDisconnected(jid);
                    break;
                default:
                    break;
            }
        }
    }

    void GLFWInput::addListener(InputListener &listener) {
        if (listeners.find(&listener) != listeners.end())
            throw std::runtime_error("Input listener already registered");
        listeners.insert(&listener);
    }

    void GLFWInput::removeListener(InputListener &listener) {
        listeners.erase(&listener);
    }

    //TODO: Implement clipboard support
    void GLFWInput::setClipboardText(std::string text) {
        throw std::runtime_error("Not Implemented");
    }

    std::string GLFWInput::getClipboardText() {
        throw std::runtime_error("Not Implemented");
    }

    bool GLFWInput::getKey(keyboard::Key key) {
        return glfwGetKey(&wndH, GLFWTypeConverter::convertKey(key)) == GLFW_PRESS;
    }

    bool GLFWInput::getMouseButton(mouse::Button key) {
        return glfwGetMouseButton(&wndH, GLFWTypeConverter::convertMouseKey(key)) == GLFW_PRESS;
    }

    Vec2d GLFWInput::getMousePosition() {
        Vec2d ret;
        glfwGetCursorPos(&wndH, &ret.x, &ret.y);
        return ret;
    }

    std::set<int> GLFWInput::getGamepads() {
        return gamepads;
    }

    std::string GLFWInput::getGamepadName(int id) {
        return glfwGetGamepadName(id);
    }

    float GLFWInput::getGamepadAxis(int id, gamepad::Axis axis) {
        GLFWgamepadstate state;
        if (!glfwGetGamepadState(id, &state)) {
            throw std::runtime_error("Failed to get axis for gamepad " + std::to_string(id));
        }
        return state.axes[GLFWTypeConverter::convertGamepadAxis(axis)];
    }

    bool GLFWInput::getGamepadButton(int id, gamepad::Button button) {
        GLFWgamepadstate state;
        if (!glfwGetGamepadState(id, &state)) {
            throw std::runtime_error("Failed to get button for gamepad " + std::to_string(id));
        }
        return state.buttons[GLFWTypeConverter::convertGamepadButton(button)];
    }

    //TODO: Implement cursor image change
    void GLFWInput::setMouseCursorImage(const Image<ColorRGBA> &image) {
        throw std::runtime_error("Not implemented");
    }

    void GLFWInput::clearMouseCursorImage() {
        throw std::runtime_error("Not implemented");
    }
}

#endif