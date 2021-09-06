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

#ifndef MANA_ECSSCHEMA_HPP
#define MANA_ECSSCHEMA_HPP

#include "engine/io/message.hpp"

#include "runtime/ecs/component.hpp"
#include "runtime/ecs/scene.hpp"

namespace engine::runtime {
    ComponentType &operator<<(ComponentType &value, const Message &message);

    Message &operator<<(Message &message, ComponentType type);

    CameraType &operator<<(CameraType &value, const Message &message);

    Message &operator<<(Message &message, CameraType type);

    LightType &operator<<(LightType &value, const Message &message);

    Message &operator<<(Message &message, LightType type);

    AssetPath &operator<<(AssetPath &value, const Message &message);

    Message &operator<<(Message &message, const AssetPath& path);

    CameraComponent *&operator<<(CameraComponent *&component, const Message &message);

    Message &operator<<(Message &message, const CameraComponent *&component);

    TransformComponent *&operator<<(TransformComponent *&component, const Message &message);

    Message &operator<<(Message &message, const TransformComponent *&component);

    MeshRenderComponent *&operator<<(MeshRenderComponent *&component, const Message &message);

    Message &operator<<(Message &message, const MeshRenderComponent *&component);

    SkyboxComponent *&operator<<(SkyboxComponent *&component, const Message &message);

    Message &operator<<(Message &message, const SkyboxComponent *&component);

    LightComponent *&operator<<(LightComponent *&component, const Message &message);

    Message &operator<<(Message &message, const LightComponent *&component);

    MonoScriptComponent *&operator<<(MonoScriptComponent *&component, const Message &message);

    Message &operator<<(Message &message, const MonoScriptComponent *&component);

    Component *&operator<<(Component *&, const Message &message);

    Message &operator<<(Message &message, const Component *&component);

    Node &operator<<(Node &node, const Message &message);

    Message &operator<<(Message &message, const Node &node);

    Scene &operator<<(Scene &scene, const Message &message);

    Message &operator<<(Message &message, const Scene &scene);
}

#endif //MANA_ECSSCHEMA_HPP
