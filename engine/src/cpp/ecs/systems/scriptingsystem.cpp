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
    MonoCppObject uploadVector(MonoCppAssembly &manaAssembly, Vec3f vec) {
        auto ret = manaAssembly.createObject("Mana", "Vector3");
        MonoCppValue v;
        v.ptr = &vec.x;
        ret.setField("x", v);
        v.ptr = &vec.y;
        ret.setField("y", v);
        v.ptr = &vec.z;
        ret.setField("z", v);
        return ret;
    }

    MonoCppObject uploadTransform(Component &c, MonoCppAssembly &manaAssembly) {
        auto &t = dynamic_cast<TransformComponent &>(c);
        MonoCppObject ret = manaAssembly.createObject("Mana", "Transform");
        MonoCppValue v;
        v.ptr = uploadVector(manaAssembly, t.transform.position).objectPointer;
        ret.setField("position", v);
        v.ptr = uploadVector(manaAssembly, t.transform.rotation).objectPointer;
        ret.setField("rotation", v);
        v.ptr = uploadVector(manaAssembly, t.transform.scale).objectPointer;
        ret.setField("scale", v);
        return ret;
    }

    MonoCppObject uploadCamera(Component &c, MonoCppAssembly &manaAssembly) {
        auto &cam = dynamic_cast<CameraComponent &>(c);
        MonoCppObject ret = manaAssembly.createObject("Mana", "Camera");
        MonoCppValue v;
        v.setValue<int>((int *) &cam.cameraType);
        ret.setField("cameraType", v);
        v.setValue<float>(&cam.nearClip);
        ret.setField("nearClip", v);
        v.setValue<float>(&cam.farClip);
        ret.setField("farClip", v);
        v.setValue<float>(&cam.left);
        ret.setField("left", v);;
        v.setValue<float>(&cam.top);
        ret.setField("top", v);
        v.setValue<float>(&cam.right);
        ret.setField("right", v);
        v.setValue<float>(&cam.bottom);
        ret.setField("bottom", v);
        v.setValue<float>(&cam.fov);
        ret.setField("fov", v);
        v.setValue<float>(&cam.aspectRatio);
        ret.setField("aspectRatio", v);
        return ret;
    }

    MonoCppObject uploadLight(Component &c, MonoCppAssembly &manaAssembly) {
        auto &l = dynamic_cast<LightComponent &>(c);
        MonoCppObject ret = manaAssembly.createObject("Mana", "Light");
        MonoCppValue v;
        v.setValue<int>((int *) &l.lightType);
        ret.setField("lightType", v);

        v.setValue<void>(uploadVector(manaAssembly, l.ambient).objectPointer);
        ret.setField("ambient", v);
        v.setValue<void>(uploadVector(manaAssembly, l.diffuse).objectPointer);
        ret.setField("diffuse", v);
        v.setValue<void>(uploadVector(manaAssembly, l.specular).objectPointer);
        ret.setField("specular", v);

        v.setValue<void>(uploadVector(manaAssembly, l.direction).objectPointer);
        ret.setField("direction", v);

        v.setValue<float>(&l.cutOff);
        ret.setField("cutOff", v);

        v.setValue<float>(&l.outerCutOff);
        ret.setField("outerCutOff", v);

        v.setValue<float>(&l.constant);
        ret.setField("constant", v);
        v.setValue<float>(&l.linear);
        ret.setField("linear", v);
        v.setValue<float>(&l.quadratic);
        ret.setField("quadratic", v);

        return ret;
    }

    MonoCppObject uploadComponent(Component &c, MonoCppAssembly &manaAssembly) {
        switch (c.componentType) {
            case Component::TRANSFORM:
                return uploadTransform(c, manaAssembly);
            case Component::CAMERA:
                return uploadCamera(c, manaAssembly);
            case Component::LIGHT:
                return uploadLight(c, manaAssembly);
            case Component::RENDER:
            case Component::SCRIPT:
            default:
                return manaAssembly.createObject("Mana", "Component");
        }
    }

    void downloadVector(MonoCppObject &o, Vec3f &vec) {
        vec.x = o.getFieldValue("x", sizeof(float)).getValue<float>();
        vec.y = o.getFieldValue("y", sizeof(float)).getValue<float>();
        vec.z = o.getFieldValue("z", sizeof(float)).getValue<float>();
    }

    void downloadTransform(MonoCppObject &o, TransformComponent &c) {
        auto mo = o.getField("position");
        downloadVector(mo, c.transform.position);
        mo = o.getField("rotation");
        downloadVector(mo, c.transform.rotation);
        mo = o.getField("scale");
        downloadVector(mo, c.transform.scale);
    }

    void downloadCamera(MonoCppObject &o, CameraComponent &c) {
        c.cameraType = o.getFieldValue("cameraType", sizeof(int)).getValue<CameraType>();
        c.nearClip = o.getFieldValue("nearClip", sizeof(float)).getValue<float>();
        c.farClip = o.getFieldValue("farClip", sizeof(float)).getValue<float>();

        c.left = o.getFieldValue("left", sizeof(float)).getValue<float>();
        c.top = o.getFieldValue("top", sizeof(float)).getValue<float>();
        c.right = o.getFieldValue("right", sizeof(float)).getValue<float>();
        c.bottom = o.getFieldValue("bottom", sizeof(float)).getValue<float>();

        c.fov = o.getFieldValue("fov", sizeof(float)).getValue<float>();
        c.aspectRatio = o.getFieldValue("aspectRatio", sizeof(float)).getValue<float>();
    }

    void downloadLight(MonoCppObject &o, LightComponent &c) {
        c.lightType = o.getFieldValue("lightType", sizeof(int)).getValue<LightType>();

        auto vo = o.getField("ambient");
        downloadVector(vo, c.ambient);

        vo = o.getField("diffuse");
        downloadVector(vo, c.diffuse);

        vo = o.getField("specular");
        downloadVector(vo, c.specular);

        vo = o.getField("direction");
        downloadVector(vo, c.direction);

        c.cutOff = o.getFieldValue("cutOff", sizeof(float)).getValue<float>();
        c.outerCutOff = o.getFieldValue("outerCutOff", sizeof(float)).getValue<float>();
        c.constant = o.getFieldValue("constant", sizeof(float)).getValue<float>();
        c.linear = o.getFieldValue("linear", sizeof(float)).getValue<float>();
        c.quadratic = o.getFieldValue("quadratic", sizeof(float)).getValue<float>();
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
        auto o = manaAssembly.createObject("Mana", "Scene");
        for (auto &n : scene.nodes) {
            auto monoNode = manaAssembly.createObject("Mana", "Node");
            for (auto &c : n.second.components) {
                auto monoComponent = uploadComponent(*c.second, manaAssembly);
                MonoCppValue v;
                v.setValue<void>(monoNode.objectPointer);
                monoComponent.setField("node", v);
                MonoCppArguments args;
                args.addArgument<void>(monoComponent.objectPointer);
                monoNode.invokeMethod("AddComponent", args);
            }
            MonoCppArguments args;
            args.addArgument<void>(runtime.getMonoStringObject(n.first).objectPointer);
            args.addArgument<void>(monoNode.objectPointer);
            o.invokeMethod("AddNode", args);
        }
        MonoCppValue v;
        v.setValue<void>(o.objectPointer);
        manaAssembly.setStaticField("Mana", "Scene", "scene", v);
    }

    void readSceneFromMono(MonoCppRuntime &runtime,
                           MonoCppAssembly &msCorLib,
                           MonoCppAssembly &manaAssembly,
                           Scene &scene) {
        auto op = manaAssembly.getStaticField("Mana", "Scene", "scene");
        auto o = MonoCppObject(op.ptr);
        for (auto &n : scene.nodes) {
            MonoCppArguments arg;
            arg.addArgument<void>(runtime.getMonoStringObject(n.first).objectPointer);
            auto nodeObject = o.invokeMethod("GetNode", arg);
            //Only read back transforms for now.
            auto transform = nodeObject.invokeMethod("GetTransform");
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