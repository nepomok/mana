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

#ifndef MANA_RENDERCOMPONENT_HPP
#define MANA_RENDERCOMPONENT_HPP

#include "engine/ecs/component.hpp"

#include "engine/render/rendercommand.hpp"

#include "engine/resource/render/meshbufferresource.hpp"
#include "engine/resource/render/shaderresource.hpp"
#include "engine/resource/render/texturebufferresource.hpp"

namespace mana {
    struct RenderComponent : public Component {
        RenderComponent() : Component(RENDER) {}

        Component *clone() override {
            return new RenderComponent(*this);
        }

        const std::type_info &getTypeInfo() override {
            return typeid(RenderComponent);
        }

        ShaderResource *shader;

        std::vector<MeshBufferResource *> meshBuffers;
        std::vector<TextureBufferResource *> textureBuffers;

        std::map<std::string, int> textureMapping;

        RenderProperties renderProperties;

        int renderOrder = 0;
    };
}

#endif //MANA_RENDERCOMPONENT_HPP
