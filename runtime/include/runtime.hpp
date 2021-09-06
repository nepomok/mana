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

#ifndef MANA_RUNTIME_HPP
#define MANA_RUNTIME_HPP

#include "runtime/application.hpp"
#include "runtime/io/schema/ecsschema.hpp"
#include "runtime/io/schema/mathschema.hpp"
#include "runtime/io/archive/directoryarchive.hpp"
#include "runtime/io/archive/packedarchive.hpp"
#include "runtime/ecs/component.hpp"
#include "runtime/ecs/componenttype.hpp"
#include "runtime/ecs/node.hpp"
#include "runtime/ecs/components.hpp"
#include "runtime/ecs/system.hpp"
#include "runtime/ecs/scene.hpp"
#include "runtime/ecs/ecs.hpp"
#include "runtime/ecs/event/event.hpp"
#include "runtime/ecs/event/eventbus.hpp"
#include "runtime/ecs/event/scene/nodecreateevent.hpp"
#include "runtime/ecs/event/scene/componentdestroyevent.hpp"
#include "runtime/ecs/event/scene/nodedestroyevent.hpp"
#include "runtime/ecs/event/scene/componentcreateevent.hpp"
#include "runtime/ecs/event/physics/contactevent.hpp"
#include "runtime/ecs/components/collidercomponent2d.hpp"
#include "runtime/ecs/components/collidercomponent3d.hpp"
#include "runtime/ecs/components/cameracomponent.hpp"
#include "runtime/ecs/components/monoscriptcomponent.hpp"
#include "runtime/ecs/components/monosynccomponent.hpp"
#include "runtime/ecs/components/transformcomponent.hpp"
#include "runtime/ecs/components/lightcomponent.hpp"
#include "runtime/ecs/components/particlecomponent.hpp"
#include "runtime/ecs/components/rigidbodycomponent3d.hpp"
#include "runtime/ecs/components/rigidbodycomponent2d.hpp"
#include "runtime/ecs/components/skyboxcomponent.hpp"
#include "runtime/ecs/components/meshrendercomponent.hpp"
#include "runtime/ecs/components/gui/textcomponent.hpp"
#include "runtime/ecs/components/gui/canvascomponent.hpp"
#include "runtime/ecs/components/gui/buttoncomponent.hpp"
#include "runtime/ecs/systems/physics2dsystem.hpp"
#include "runtime/ecs/systems/animationsystem.hpp"
#include "runtime/ecs/systems/eventsystem.hpp"
#include "runtime/ecs/systems/monoscriptingsystem.hpp"
#include "runtime/ecs/systems/physics3dsystem.hpp"
#include "runtime/ecs/systems/rendersystem.hpp"
#include "runtime/script/script.hpp"
#include "runtime/script/mono/monocppdomain.hpp"
#include "runtime/script/mono/monocppobject.hpp"
#include "runtime/script/mono/monocppvalue.hpp"
#include "runtime/script/mono/monoscript.hpp"
#include "runtime/script/mono/monocpparguments.hpp"
#include "runtime/script/mono/monocppassembly.hpp"

#endif //MANA_RUNTIME_HPP
