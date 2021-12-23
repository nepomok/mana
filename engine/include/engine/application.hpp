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

#ifndef MANA_APPLICATION_HPP
#define MANA_APPLICATION_HPP

#include <chrono>
#include <thread>

#include "engine/display/displaymanager.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/io/archive.hpp"

namespace engine {
    class Application {
    public:
        explicit Application(int argc, char *argv[], std::unique_ptr<Archive> archive)
                : archive(std::move(archive)), display() {
            std::vector<std::string> args;
            for (int i = 0; i < argc; i++)
                args.emplace_back(argv[i]);

            displayBackend = DisplayBackend::GLFW;
            for (int i = 0; i < args.size(); i++) {
                if (args.at(i) == "--display") {
                    auto str = args.at(i + 1);
                    if (str == "glfw") {
                        displayBackend = DisplayBackend::GLFW;
                    }
                }
            }
            display = DisplayManager(displayBackend);

            graphicsBackend = GraphicsBackend::OPENGL_4_6;
            for (int i = 0; i < args.size(); i++) {
                if (args.at(i) == "--graphics") {
                    auto str = args.at(i + 1);
                    if (str == "opengl") {
                        graphicsBackend = GraphicsBackend::OPENGL_4_6;
                    } else if (str == "directx") {
                        graphicsBackend = GraphicsBackend::DIRECTX_11;
                    } else if (str == "vulkan") {
                        graphicsBackend = GraphicsBackend::VULKAN;
                    }
                }
            }
            window = display.createWindow(graphicsBackend);
        }

        virtual ~Application() = default;

        virtual int loop() {
            start();
            auto lastFrame = std::chrono::high_resolution_clock::now();
            while (!window->shouldClose()) {
                auto frame = std::chrono::high_resolution_clock::now();
                update(mDeltaTime);
                if (fpsLimit != 0) {
                    auto delta = std::chrono::high_resolution_clock::now() - frame;
                    auto time = std::chrono::nanoseconds(static_cast<long>(1000000000.0f / fpsLimit));
                    std::this_thread::sleep_for(time - delta);
                }
                auto frameDelta = std::chrono::high_resolution_clock::now() - frame;
                mDeltaTime = static_cast<float>(frameDelta.count()) / 1000000000.0f;
            }
            stop();
            return 0;
        }

    protected:
        DisplayManager display;
        ECS ecs;

        float fpsLimit = 0;
        float mDeltaTime = 0;

        DisplayBackend displayBackend;
        GraphicsBackend graphicsBackend;

        std::unique_ptr<Archive> archive = nullptr;
        std::unique_ptr<Window> window = nullptr;

        virtual void start() {}

        virtual void stop() {}

        virtual void update(float deltaTime) {
            window->update();
            ecs.update(deltaTime);
            window->swapBuffers();
        }
    };
}

#endif //MANA_APPLICATION_HPP
