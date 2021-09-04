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

#include <algorithm>
#include <sstream>

#include "engine/ecs/systems/scriptingsystem.hpp"
#include "engine/ecs/components.hpp"
#include "engine/io/json/ecs/sceneserializer.hpp"
#include "engine/io/json/ecs/scenedeserializer.hpp"

#include "script/sceneinterface.hpp"

namespace mana {
    void uploadScene(MonoCppDomain &runtime,
                     MonoCppAssembly &msCorLib,
                     MonoCppAssembly &manaAssembly,
                     const Scene &scene) {
        MonoCppArguments args;
        std::stringstream stream;
        SceneSerializer().serialize(scene, stream);

        std::string s = stream.str();

        MonoCppObject str = runtime.stringFromUtf8(s, true);
        args.add(str);
        manaAssembly.invokeStaticMethod("Mana.Internal", "SceneInterface", "setSceneJson", args);
    }

    void downloadScene(MonoCppDomain &domain,
                       MonoCppAssembly &msCorLib,
                       MonoCppAssembly &manaAssembly,
                       const Scene &scene) {
        auto str = manaAssembly.invokeStaticMethod("Mana.Internal", "SceneInterface", "getSceneJson");
        std::stringstream stream(domain.stringToUtf8(str));
        auto monoScene = SceneDeserializer().deserialize(stream);

        //Synchronize transforms
        for (auto &node : monoScene.nodes) {
            if (!node.second.hasComponent<TransformComponent>())
                continue;
            auto &comp = node.second.getComponent<TransformComponent>();
            scene.nodes.at(node.first).getComponent<TransformComponent>().transform = comp.transform;
            scene.nodes.at(node.first).getComponent<TransformComponent>().parent = comp.parent;
        }
    }

    ScriptingSystem::ScriptingSystem(Input &input,
                                     MonoCppDomain &domain,
                                     MonoCppAssembly &manaAssembly,
                                     Archive &archive)
            : domain(domain),
              msCorLib(domain.getMsCorLibAssembly()),
              manaAssembly(manaAssembly),
              input(input),
              archive(archive) {}

    void ScriptingSystem::start() {
        input.registerListener(*this);
    }

    void ScriptingSystem::stop() {
        input.unregisterListener(*this);
    }

    void ScriptingSystem::update(float deltaTime, Scene &scene) {
        manaAssembly.setStaticField("Mana", "Time", "deltaTime", &deltaTime);

        uploadScene(domain, msCorLib, manaAssembly, scene);

        auto nodes = scene.findNodesWithComponent<ScriptComponent>();

        std::sort(nodes.begin(), nodes.end(),
                  [](const Node *a, const Node *b) -> bool {
                      return a->getComponent<ScriptComponent>().queue < b->getComponent<ScriptComponent>().queue;
                  });

        std::set<std::string> usedAssemblies;

        for (auto *node : nodes) {
            auto &comp = node->getComponent<ScriptComponent>();

            if (comp.runtime == "mono") {
                auto *script = reinterpret_cast<MonoScript *>(comp.userData.get());

                usedAssemblies.insert(comp.assembly);

                if (script == nullptr) {
                    auto &assembly = getAssembly(comp.assembly);
                    script = new MonoScript(&assembly, comp.nameSpace, comp.className);
                    comp.userData = std::shared_ptr<MonoScript>(script);
                }

                if (!node->enabled || !comp.enabled) {
                    if (comp.scriptEnabled) {
                        comp.scriptEnabled = false;
                        script->onDisable();
                    }
                }
                if (!comp.scriptEnabled) {
                    comp.scriptEnabled = true;
                    script->onEnable();
                }
                script->onUpdate();
            }
        }

        std::set<std::string> unused;
        for (auto &pair : assemblies) {
            if (usedAssemblies.find(pair.first) == usedAssemblies.end())
                unused.insert(pair.first);
        }
        for (auto &s : unused) {
            assemblies.erase(s);
        }

        downloadScene(domain, msCorLib, manaAssembly, scene);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnFrameEnd");
    }

    void ScriptingSystem::onKeyDown(Key key) {
        MonoCppArguments args;
        int v = key;
        args.add(v);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnKeyDown", args);
    }

    void ScriptingSystem::onKeyUp(Key key) {
        MonoCppArguments args;
        int v = key;
        args.add(v);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnKeyUp", args);
    }

    void ScriptingSystem::onMouseMove(double xpos, double ypos) {
        MonoCppArguments args;
        args.add(xpos);
        args.add(ypos);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnMouseMove", args);
    }

    void ScriptingSystem::onMouseWheelScroll(double ammount) {
        MonoCppArguments args;
        args.add(ammount);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnMouseWheelScroll", args);
    }

    void ScriptingSystem::onMouseKeyDown(MouseKey key) {
        int v = key;
        MonoCppArguments args;
        args.add(v);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnMouseKeyDown", args);
    }

    void ScriptingSystem::onMouseKeyUp(MouseKey key) {
        int v = key;
        MonoCppArguments args;
        args.add(v);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnMouseKeyUp", args);
    }

    void ScriptingSystem::onTextInput(const std::string &text) {
        MonoCppArguments args;
        auto monostr = domain.stringFromUtf8(text);
        args.add(monostr);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnTextInput", args);
    }

    MonoCppAssembly &ScriptingSystem::getAssembly(const std::string &path) {
        if (assemblies.find(path) == assemblies.end()) {
            std::unique_ptr<std::iostream> stream(archive.open(path));
            assemblies[path] = domain.loadAssembly(*stream);
        }
        return *assemblies[path];
    }
}