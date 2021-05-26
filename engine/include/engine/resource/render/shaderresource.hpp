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

#ifndef MANA_SHADERRESOURCE_HPP
#define MANA_SHADERRESOURCE_HPP

#include "engine/resource/resource.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    class ShaderResource : public Resource<ShaderProgram> {
    public:
        ShaderResource();

        ShaderResource(RenderAllocator &alloc,
                       Resource <std::string> &vertexShader,
                       Resource <std::string> &fragmentShader);

        ~ShaderResource() override;

        void load() override;

        void free() override;

        ShaderProgram &get() override;

    private:
        RenderAllocator *alloc;
        Resource <std::string> *vertexShader;
        Resource <std::string> *fragmentShader;
        ShaderProgram *shader;
        bool isLoaded = false;
    };
}
#endif //MANA_SHADERRESOURCE_HPP
