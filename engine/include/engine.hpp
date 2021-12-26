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

#ifndef MANA_ENGINE_HPP
#define MANA_ENGINE_HPP

#include "engine/application.hpp"
#include "engine/ecs/componentmanager.hpp"
#include "engine/ecs/components.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/ecs/entitymanager.hpp"
#include "engine/ecs/system.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/ecs/componentpool.hpp"
#include "engine/ecs/systems/animationsystem.hpp"
#include "engine/ecs/systems/physics3dsystem.hpp"
#include "engine/ecs/systems/rendersystem.hpp"
#include "engine/ecs/systems/monoscriptingsystem.hpp"
#include "engine/ecs/systems/audiosystem.hpp"
#include "engine/ecs/systems/physics2dsystem.hpp"
#include "engine/ecs/systems/eventsystem.hpp"
#include "engine/ecs/components/monoscriptcomponent.hpp"
#include "engine/ecs/components/meshrendercomponent.hpp"
#include "engine/ecs/components/skyboxcomponent.hpp"
#include "engine/ecs/components/cameracomponent.hpp"
#include "engine/ecs/components/collidercomponent2d.hpp"
#include "engine/ecs/components/particlecomponent.hpp"
#include "engine/ecs/components/monosynccomponent.hpp"
#include "engine/ecs/components/collidercomponent3d.hpp"
#include "engine/ecs/components/rigidbodycomponent3d.hpp"
#include "engine/ecs/components/transformcomponent.hpp"
#include "engine/ecs/components/lightcomponent.hpp"
#include "engine/ecs/components/rigidbodycomponent2d.hpp"
#include "engine/ecs/components/gui/buttoncomponent.hpp"
#include "engine/ecs/components/gui/textcomponent.hpp"
#include "engine/ecs/components/gui/canvascomponent.hpp"
#include "engine/ecs/components/audio/audiosourcecomponent.hpp"
#include "engine/ecs/components/audio/audiolistenercomponent.hpp"
#include "engine/schema/ecsschema.hpp"
#include "engine/schema/mathschema.hpp"
#include "engine/render/2d/renderer2d.hpp"
#include "engine/render/deferred/geometrybuffer.hpp"
#include "engine/render/deferred/compositor.hpp"
#include "engine/render/deferred/renderpass.hpp"
#include "engine/render/deferred/deferredrenderer.hpp"
#include "engine/render/deferred/passes/forwardpass.hpp"
#include "engine/render/deferred/passes/phongshadepass.hpp"
#include "engine/render/deferred/passes/skyboxpass.hpp"
#include "engine/render/deferred/passes/outlinepass.hpp"
#include "engine/render/deferred/passes/shadowpass.hpp"
#include "engine/render/deferred/passes/imguipass.hpp"
#include "engine/render/deferred/passes/prepass.hpp"
#include "engine/render/deferred/passes/debugpass.hpp"
#include "engine/render/forward/forwardrenderer.hpp"
#include "engine/render/shader/shaderinclude.hpp"
#include "engine/asset/assetbundle.hpp"
#include "engine/asset/skybox.hpp"
#include "engine/asset/assetexporter.hpp"
#include "engine/asset/audio.hpp"
#include "engine/asset/light.hpp"
#include "engine/asset/assetpath.hpp"
#include "engine/asset/assetimporter.hpp"
#include "engine/asset/camera.hpp"
#include "engine/asset/shader.hpp"
#include "engine/asset/assethandle.hpp"
#include "engine/asset/texture.hpp"
#include "engine/asset/material.hpp"
#include "engine/asset/asset.hpp"
#include "engine/asset/scene.hpp"
#include "engine/asset/manager/assetrendermanager.hpp"
#include "engine/asset/manager/assetmanager.hpp"
#include "engine/event/eventbus.hpp"
#include "engine/event/eventreceiver.hpp"

#endif // MANA_ENGINE_HPP