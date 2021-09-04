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

#ifndef MANA_JSONCOMMON_HPP
#define MANA_JSONCOMMON_HPP

#include "engine/ecs/scene.hpp"

#include "../../../extern/json.hpp"

using namespace mana;

ComponentType convertComponentType(const std::string &str);

std::string convertComponentType(ComponentType type);

CameraType convertCameraType(const std::string &str);

std::string convertCameraType(CameraType type);

LightType convertLightType(const std::string &str);

std::string convertLightType(LightType type);

Vec3f convertVector3(const nlohmann::json &v);

nlohmann::json convertVector3(const Vec3f &v);

Vec4f convertVector4(const nlohmann::json &v);

nlohmann::json convertVector4(const Vec4f &v);

Transform convertTransform(const nlohmann::json &component);

nlohmann::json convertTransform(const Transform &transform);

AssetPath convertAssetPath(const nlohmann::json &path);

nlohmann::json convertAssetPath(AssetPath path);

CameraComponent *convertCameraComponent(const nlohmann::json &component);

nlohmann::json convertCameraComponent(const CameraComponent &component);

TransformComponent *convertTransformComponent(const nlohmann::json &component);

nlohmann::json convertTransformComponent(const TransformComponent &component);

MeshRenderComponent *convertMeshRenderComponent(const nlohmann::json &component);

nlohmann::json convertMeshRenderComponent(const MeshRenderComponent &component);

SkyboxComponent *convertSkyboxComponent(const nlohmann::json &component);

nlohmann::json convertSkyboxComponent(const SkyboxComponent &component);

LightComponent *convertLightComponent(const nlohmann::json &component);

nlohmann::json convertLightComponent(const LightComponent &component);

ScriptComponent *convertScriptComponent(const nlohmann::json &component);

nlohmann::json convertScriptComponent(const ScriptComponent &component);

Component *convertComponent(const nlohmann::json &component);

nlohmann::json convertComponent(const Component &component);

Node convertNode(const nlohmann::json &node);

nlohmann::json convertNode(const Node &node);

Scene convertScene(const nlohmann::json &scene);

nlohmann::json convertScene(const Scene &scene);

#endif //MANA_JSONCOMMON_HPP
