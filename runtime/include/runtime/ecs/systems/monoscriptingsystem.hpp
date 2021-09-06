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

#ifndef MANA_SCRIPTINGSYSTEM_H
#define MANA_SCRIPTINGSYSTEM_H

#include "../system.hpp"

#include "engine/script/mono/monoscript.hpp"
#include "engine/script/mono/monocppassembly.hpp"

#include "engine/input/input.hpp"

#include "engine/io/archive.hpp"

namespace mana {
    class MonoScriptingSystem : public System, InputListener {
    public:
        MonoScriptingSystem(Input &input,
                            MonoCppDomain &domain,
                            MonoCppAssembly &manaAssembly,
                            Archive &archive);

        ~MonoScriptingSystem() override = default;

        void start() override;

        void stop() override;

        void update(float deltaTime, Scene &scene) override;

    private:
        void onKeyDown(Key key) override;

        void onKeyUp(Key key) override;

        void onMouseMove(double xPos, double yPos) override;

        void onMouseWheelScroll(double amount) override;

        void onMouseKeyDown(MouseKey key) override;

        void onMouseKeyUp(MouseKey key) override;

        void onTextInput(const std::string &text) override;

    private:
        Input &input;
        Archive &archive;
        MonoCppDomain &domain;
        MonoCppAssembly &msCorLib;
        MonoCppAssembly &manaAssembly;

        MonoCppAssembly &getAssembly(const std::string &path);

        std::map<std::string, MonoCppAssembly*> assemblies;
    };
}

#endif //MANA_SCRIPTINGSYSTEM_H
