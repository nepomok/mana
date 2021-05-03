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

//TODO: Refactor script scene interface
namespace mana {
    MonoCppObject uploadVector(MonoCppAssembly &manaAssembly, Vec3f vec) {
        auto ret = manaAssembly.createObject("Mana", "Vector3");
        ret.setField("x", &vec.x);
        ret.setField("y", &vec.y);
        ret.setField("z", &vec.z);
        return std::move(ret);
    }

    MonoCppObject uploadTransform(Component &c, MonoCppAssembly &manaAssembly) {
        auto &t = dynamic_cast<TransformComponent &>(c);
        auto ret = manaAssembly.createObject("Mana", "Transform");
        ret.setField("position", uploadVector(manaAssembly, t.transform.position).getObjectPointer());
        ret.setField("rotation", uploadVector(manaAssembly, t.transform.rotation).getObjectPointer());
        ret.setField("scale", uploadVector(manaAssembly, t.transform.scale).getObjectPointer());
        return std::move(ret);
    }

    MonoCppObject uploadCamera(Component &c, MonoCppAssembly &manaAssembly) {
        auto &cam = dynamic_cast<CameraComponent &>(c);
        auto ret = manaAssembly.createObject("Mana", "Camera");
        ret.setField("cameraType", &cam.cameraType);
        ret.setField("nearClip", &cam.nearClip);
        ret.setField("farClip", &cam.farClip);
        ret.setField("left", &cam.farClip);;
        ret.setField("top", &cam.farClip);
        ret.setField("right", &cam.farClip);
        ret.setField("bottom", &cam.farClip);
        ret.setField("fov", &cam.farClip);
        ret.setField("aspectRatio", &cam.farClip);
        return std::move(ret);
    }

    MonoCppObject uploadLight(Component &c, MonoCppAssembly &manaAssembly) {
        auto &l = dynamic_cast<LightComponent &>(c);
        auto ret = manaAssembly.createObject("Mana", "Light");
        ret.setField("lightType", &l.lightType);

        ret.setField("ambient", uploadVector(manaAssembly, l.ambient).getObjectPointer());
        ret.setField("diffuse", uploadVector(manaAssembly, l.diffuse).getObjectPointer());
        ret.setField("specular", uploadVector(manaAssembly, l.specular).getObjectPointer());
        ret.setField("direction", uploadVector(manaAssembly, l.direction).getObjectPointer());

        ret.setField("cutOff", &l.cutOff);
        ret.setField("outerCutOff", &l.outerCutOff);
        ret.setField("constant", &l.constant);
        ret.setField("linear", &l.linear);
        ret.setField("quadratic", &l.quadratic);

        return std::move(ret);
    }

    MonoCppObject uploadComponent(Component &c, MonoCppAssembly &manaAssembly) {
        switch (c.componentType) {
            case Component::TRANSFORM:
                return std::move(uploadTransform(c, manaAssembly));
            case Component::CAMERA:
                return std::move(uploadCamera(c, manaAssembly));
            case Component::LIGHT:
                return std::move(uploadLight(c, manaAssembly));
            case Component::RENDER:
            case Component::SCRIPT:
            default:
                return std::move(manaAssembly.createObject("Mana", "Component"));
        }
    }

    void downloadVector(MonoCppObject &o, Vec3f &vec) {
        vec.x = o.getField<float>("x");
        vec.y = o.getField<float>("y");
        vec.z = o.getField<float>("z");
    }

    void downloadTransform(MonoCppObject &o, TransformComponent &c) {
        auto mo = o.getField("position");
        downloadVector(mo, c.transform.position);
        auto ro = o.getField("rotation");
        downloadVector(ro, c.transform.rotation);
        auto so = o.getField("scale");
        downloadVector(so, c.transform.scale);
    }

    void downloadCamera(MonoCppObject &o, CameraComponent &c) {
        c.cameraType = o.getField<CameraType>("cameraType");
        c.nearClip = o.getField<float>("nearClip");
        c.farClip = o.getField<float>("farClip");

        c.left = o.getField<float>("left");
        c.top = o.getField<float>("top");
        c.right = o.getField<float>("right");
        c.bottom = o.getField<float>("bottom");

        c.fov = o.getField<float>("fov");
        c.aspectRatio = o.getField<float>("aspectRatio");
    }

    void downloadLight(MonoCppObject &o, LightComponent &c) {
        c.lightType = o.getField<LightType>("lightType");

        auto ot = o.getField("ambient");
        downloadVector(ot, c.ambient);

        ot = o.getField("diffuse");
        downloadVector(ot, c.diffuse);

        ot = o.getField("specular");
        downloadVector(ot, c.specular);

        ot = o.getField("direction");
        downloadVector(ot, c.direction);

        c.cutOff = o.getField<float>("cutOff");
        c.outerCutOff = o.getField<float>("outerCutOff");
        c.constant = o.getField<float>("constant");
        c.linear = o.getField<float>("linear");
        c.quadratic = o.getField<float>("quadratic");
    }

    void downloadComponent(MonoCppObject &o, Component &c) {
        switch (c.componentType) {
            case Component::TRANSFORM:
                downloadTransform(o, (TransformComponent &) c);
                break;
            case Component::CAMERA:
                downloadCamera(o, (CameraComponent &) c);
                break;
            case Component::LIGHT:
                downloadLight(o, (LightComponent &) c);
                break;
            case Component::RENDER:
            case Component::SCRIPT:
                break;
        }
    }

    void applySceneToMono(MonoCppRuntime &runtime,
                          MonoCppAssembly &msCorLib,
                          MonoCppAssembly &manaAssembly,
                          Scene &scene) {
        auto monoScene = manaAssembly.createObject("Mana", "Scene");
        for (auto &n : scene.nodes) {
            auto monoNode = manaAssembly.createObject("Mana", "Node");
            for (auto &c : n.second.components) {
                auto monoComponent = uploadComponent(*c.second, manaAssembly);
                monoComponent.setField("node", monoNode.getObjectPointer());
                MonoCppArguments args;
                args.add(monoComponent);
                monoNode.invokeMethod("AddComponent", args);
            }
            auto str = runtime.stringFromUtf8(n.first);
            MonoCppArguments args;
            args.add(str);
            args.add(monoNode);
            monoScene.invokeMethod("AddNode", args);
        }
        manaAssembly.setStaticField("Mana", "Scene", "scene", monoScene.getObjectPointer());
    }

    void readSceneFromMono(MonoCppRuntime &runtime,
                           MonoCppAssembly &msCorLib,
                           MonoCppAssembly &manaAssembly,
                           Scene &scene) {
        auto monoScene = manaAssembly.getStaticField("Mana", "Scene", "scene");
        for (auto &n : scene.nodes) {
            auto monoStr = runtime.stringFromUtf8(n.first);
            MonoCppArguments arg;
            arg.add(monoStr);
            auto monoNode = monoScene.invokeMethod("GetNode", arg);
            //Only read back transforms for now.
            auto transform = monoNode.invokeMethod("GetTransform");
            downloadComponent(transform, n.second.getComponent<TransformComponent>());
        }
    }

    ScriptingSystem::ScriptingSystem(MonoCppRuntime &runtime, MonoCppAssembly &msCorLib, MonoCppAssembly &manaAssembly)
            : runtime(&runtime),
              msCorLib(&msCorLib),
              manaAssembly(&manaAssembly) {}

    void ScriptingSystem::start() {

    }

    void ScriptingSystem::stop() {

    }

    void ScriptingSystem::update(float deltaTime, Scene &scene) {
        applySceneToMono(*runtime, *msCorLib, *manaAssembly, scene);
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
        readSceneFromMono(*runtime, *msCorLib, *manaAssembly, scene);
    }
}