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

#include "engine/ecs/systems/scriptingsystem.hpp"

#include "engine/ecs/components.hpp"

#include <cstring>

namespace mana {
    MonoCppObject uploadComponent(const Component &c, MonoCppAssembly &manaAssembly) {
        return manaAssembly.createObject("Mana", "Transform");
    }

    Component downloadComponent(MonoCppObject &o) {

    }

    void applySceneToMono(MonoCppAssembly &msCorLib, MonoCppAssembly &manaAssembly, Scene &scene) {
        auto o = manaAssembly.createObject("Mana", "Scene");
        for (auto &n : scene.nodes) {
            auto monoNode = manaAssembly.createObject("Mana", "Node");
            for (auto &c : n.second.components) {
                auto monoComponent = uploadComponent(*c.second, manaAssembly);
                MonoCppArguments args;
                args.addArgument<void>(monoComponent.objectPointer);
                monoNode.invokeMethod("AddComponent", args);
            }
            MonoCppArguments args;
            args.addArgumentString(n.first);
            args.addArgument<void>(monoNode.objectPointer);
            o.invokeMethod("SetNode", args);
        }
        MonoCppValue v;
        v.setValue<void>(o.objectPointer);
        manaAssembly.setStaticField("Mana", "Scene", "scene", v);
    }

    void readSceneFromMono(MonoCppAssembly &msCorLib, MonoCppAssembly &manaAssembly, Scene &scene) {
        auto o = manaAssembly.getStaticField("Mana", "Scene", "scene");
        for (auto &n : scene.nodes) {
            auto nodeObject = MonoCppObject(o.ptr);
        }
    }

    ScriptingSystem::ScriptingSystem(MonoCppAssembly &msCorLib, MonoCppAssembly &manaAssembly)
            : msCorLib(&msCorLib),
              manaAssembly(&manaAssembly) {}

    void ScriptingSystem::start() {

    }

    void ScriptingSystem::stop() {

    }

    void ScriptingSystem::update(float deltaTime, Scene &scene) {
        applySceneToMono(*msCorLib, *manaAssembly, scene);
        auto nodes = scene.findNodesWithComponent<ScriptComponent>();
        for (auto *node : nodes) {
            auto &comp = node->getComponent<ScriptComponent>();
            if (!node->enabled || !comp.enabled) {
                if (comp.scriptEnabled) {
                    comp.script->onDisable();
                    comp.scriptEnabled = false;
                }
                continue;
            }
            if (!comp.scriptEnabled) {
                comp.scriptEnabled = true;
                comp.script->onEnable();
            }
            comp.script->onUpdate();
        }
        readSceneFromMono(*msCorLib, *manaAssembly, scene);
    }
}