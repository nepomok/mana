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

/**
 * Convenience header which includes all engine headers.
 */

#ifndef MANA_MANA_HPP
#define MANA_MANA_HPP

#include "engine/io/imageloader.hpp"
#include "engine/io/resource.hpp"
#include "engine/io/resourcemanager.hpp"
#include "engine/io/file.hpp"
#include "engine/io/imagewriter.hpp"
#include "engine/io/meshloader.hpp"
#include "engine/io/fileresource.hpp"
#include "engine/io/sceneloader.hpp"
#include "engine/input/input.hpp"
#include "engine/input/mousekey.hpp"
#include "engine/input/inputlistener.hpp"
#include "engine/input/key.hpp"
#include "engine/input/mouse.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/node.hpp"
#include "engine/ecs/components.hpp"
#include "engine/ecs/system.hpp"
#include "engine/ecs/scene.hpp"
#include "engine/ecs/ecs.hpp"
#include "engine/ecs/components/collidercomponent.hpp"
#include "engine/ecs/components/boneanimatorcomponent.hpp"
#include "engine/ecs/components/cameracomponent.hpp"
#include "engine/ecs/components/transformcomponent.hpp"
#include "engine/ecs/components/lightcomponent.hpp"
#include "engine/ecs/components/particlecomponent.hpp"
#include "engine/ecs/components/rigidbodycomponent.hpp"
#include "engine/ecs/components/rendercomponent.hpp"
#include "engine/ecs/components/scriptcomponent.hpp"
#include "engine/ecs/components/gui/textcomponent.hpp"
#include "engine/ecs/components/gui/canvascomponent.hpp"
#include "engine/ecs/components/gui/buttoncomponent.hpp"
#include "engine/ecs/systems/animationsystem.hpp"
#include "engine/ecs/systems/eventsystem.hpp"
#include "engine/ecs/systems/scriptingsystem.hpp"
#include "engine/ecs/systems/physicssystem.hpp"
#include "engine/ecs/systems/rendersystem.hpp"
#include "engine/common/color.hpp"
#include "engine/common/buffer.hpp"
#include "engine/common/imagebuffer.hpp"
#include "engine/sound/audiobuffer.hpp"
#include "engine/sound/audioplayer.hpp"
#include "engine/script/script.hpp"
#include "engine/display/windowlistener.hpp"
#include "engine/display/displayapi.hpp"
#include "engine/display/videomode.hpp"
#include "engine/display/windowattributes.hpp"
#include "engine/display/monitor.hpp"
#include "engine/display/window.hpp"
#include "engine/display/displaymanager.hpp"
#include "engine/render/renderobject.hpp"
#include "engine/render/graphicsapi.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/rendertarget.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/shaderprogram.hpp"
#include "engine/render/renderer2d.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/renderer3d.hpp"
#include "engine/render/renderallocator.hpp"
#include "engine/render/camera/orthographiccamera.hpp"
#include "engine/render/camera/camera.hpp"
#include "engine/render/camera/perspectivecamera.hpp"
#include "engine/render/lighting/light.hpp"
#include "engine/render/geometry/vertex.hpp"
#include "engine/render/geometry/primitive.hpp"
#include "engine/render/geometry/mesh.hpp"
#include "engine/physics/physicsengine.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/transform.hpp"
#include "engine/math/vectorconversion.hpp"
#include "engine/math/vector4.hpp"
#include "engine/math/matrix.hpp"
#include "engine/math/matrixmath.hpp"
#include "engine/math/grid.hpp"
#include "engine/math/rotation.hpp"
#include "engine/math/rectangle.hpp"
#include "engine/math/vector3.hpp"

#endif //MANA_MANA_HPP
