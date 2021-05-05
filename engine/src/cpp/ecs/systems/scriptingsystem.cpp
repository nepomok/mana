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

#include "engine/ecs/systems/scriptingsystem.hpp"

#include "engine/ecs/components.hpp"

#include "script/sceneinterface.hpp"

#include "engine/resource/scriptresource.hpp"

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
        ret.setField("left", &cam.left);;
        ret.setField("top", &cam.top);
        ret.setField("right", &cam.right);
        ret.setField("bottom", &cam.bottom);
        ret.setField("fov", &cam.fov);
        ret.setField("aspectRatio", &cam.aspectRatio);
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
            case ComponentType::TRANSFORM:
                return std::move(uploadTransform(c, manaAssembly));
            case ComponentType::CAMERA:
                return std::move(uploadCamera(c, manaAssembly));
            case ComponentType::LIGHT:
                return std::move(uploadLight(c, manaAssembly));
            case ComponentType::RENDER:
            case ComponentType::SCRIPT:
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
            case ComponentType::TRANSFORM:
                downloadTransform(o, (TransformComponent &) c);
                break;
            case ComponentType::CAMERA:
                downloadCamera(o, (CameraComponent &) c);
                break;
            case ComponentType::LIGHT:
                downloadLight(o, (LightComponent &) c);
                break;
            case ComponentType::RENDER:
            case ComponentType::SCRIPT:
            default:
                break;
        }
    }

    void uploadScene(MonoCppDomain &runtime,
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
                monoNode.invokeMethod("_AddComponent", args);
            }
            auto str = runtime.stringFromUtf8(n.first);
            MonoCppArguments args;
            args.add(str);
            args.add(monoNode);
            monoScene.invokeMethod("_AddNode", args);
        }
        manaAssembly.setStaticField("Mana", "Scene", "scene", monoScene.getObjectPointer());
    }

    void downloadScene(MonoCppDomain &domain,
                       MonoCppAssembly &msCorLib,
                       MonoCppAssembly &manaAssembly,
                       Scene &scene) {
        auto monoScene = manaAssembly.getStaticField("Mana", "Scene", "scene");
        for (auto &n : scene.nodes) {
            auto monoStr = domain.stringFromUtf8(n.first);
            MonoCppArguments arg;
            arg.add(monoStr);
            auto monoNode = monoScene.invokeMethod("GetNode", arg);
            const int componentCount = monoNode.invokeMethod("GetComponentCount").unbox<int>();
            for (int i = 0; i < componentCount; i++) {
                arg.clear();
                arg.add(i);
                auto monoComponent = monoNode.invokeMethod("GetComponent", arg);
                auto t = monoComponent.getField<ComponentType>("type");
                if (t == ComponentType::NONE)
                    continue;
                downloadComponent(monoComponent, *n.second.components.at(Node::getComponentTypeIndex(t)));
            }
        }
    }

    ScriptingSystem::ScriptingSystem(Resources &res,
                                     Input &input,
                                     MonoCppDomain &domain,
                                     MonoCppAssembly &manaAssembly)
            : res(res),
              domain(domain),
              msCorLib(domain.getMsCorLibAssembly()),
              manaAssembly(manaAssembly),
              input(input) {}

    void ScriptingSystem::start() {
        input.registerListener(*this);
    }

    void ScriptingSystem::stop() {
        input.unregisterListener(*this);
    }

    void ScriptingSystem::update(float deltaTime, Scene &scene) {
        manaAssembly.setStaticField("Mana", "Time", "deltaTime", &deltaTime);

        SceneInterface::setScene(&scene);
        uploadScene(domain, msCorLib, manaAssembly, scene);
        auto nodes = scene.findNodesWithComponent<ScriptComponent>();

        std::sort(nodes.begin(), nodes.end(),
                  [](const Node *a, const Node *b) -> bool {
                      return a->getComponent<ScriptComponent>().queue < b->getComponent<ScriptComponent>().queue;
                  });

        for (auto *node : nodes) {
            auto &comp = node->getComponent<ScriptComponent>();
            if (!node->enabled || !comp.enabled) {
                if (comp.scriptEnabled) {
                    comp.scriptEnabled = false;
                    res.getResource<ScriptResource>(comp.scriptResourceName).getScript()->onDisable();
                }
            }
            if (!comp.scriptEnabled) {
                comp.scriptEnabled = true;
                res.getResource<ScriptResource>(comp.scriptResourceName).getScript()->onEnable();
            }
            res.getResource<ScriptResource>(comp.scriptResourceName).getScript()->onUpdate();
        }

        SceneInterface::setScene(nullptr);
        downloadScene(domain, msCorLib, manaAssembly, scene);
        MonoCppArguments a;
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnFrameEnd", a);
    }

    void ScriptingSystem::onKeyDown(Key key) {
        int v = key;
        MonoCppArguments args;
        args.add(v);
        manaAssembly.invokeStaticMethod("Mana", "Input", "OnKeyDown", args);
    }

    void ScriptingSystem::onKeyUp(Key key) {
        int v = key;
        MonoCppArguments args;
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
}