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

#include "engine/display/displaymanager.hpp"

#include <utility>

#include "display/glfw/glfwdisplay.hpp"

mana::DisplayManager::DisplayManager(mana::DisplayBackend displayApi) : displayApi(displayApi) {}

mana::DisplayManager::~DisplayManager() = default;

mana::Monitor *mana::DisplayManager::getPrimaryMonitor() const {
    switch (displayApi) {
        case GLFW:
            return glfw::getPrimaryMonitor();
        default:
            throw std::runtime_error("Unsupported display api");
    }
}

std::set<mana::Monitor *> mana::DisplayManager::getMonitors() const {
    switch (displayApi) {
        case GLFW:
            return glfw::getMonitors();
        default:
            throw std::runtime_error("Unsupported display api");
    }
}

mana::Window *mana::DisplayManager::createWindow(mana::GraphicsBackend graphicsBackend) const {
    switch (displayApi) {
        case GLFW:
            return glfw::createWindow(graphicsBackend);
        default:
            throw std::runtime_error("Unsupported display api");
    }
}

mana::Window *mana::DisplayManager::createWindow(mana::GraphicsBackend graphicsBackend, std::string title, mana::Vec2i size,
                                                 mana::WindowAttributes attributes) const {
    switch (displayApi) {
        case GLFW:
            return glfw::createWindow(graphicsBackend, std::move(title), size, attributes);
        default:
            throw std::runtime_error("Unsupported display api");
    }
}

mana::Window *mana::DisplayManager::createWindow(mana::GraphicsBackend graphicsBackend, std::string title, mana::Vec2i size,
                                                 mana::WindowAttributes attributes, mana::Monitor &monitor,
                                                 mana::VideoMode mode) const {
    switch (displayApi) {
        case GLFW:
            return glfw::createWindow(graphicsBackend, std::move(title), size, attributes, monitor, mode);
        default:
            throw std::runtime_error("Unsupported display api");
    }
}
