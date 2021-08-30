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

#ifndef MANA_GEOMETRYPASS_HPP
#define MANA_GEOMETRYPASS_HPP

#include "engine/render/3d/renderpass.hpp"

namespace mana {
    class GeometryPass : public RenderPass {
    public:
        explicit GeometryPass(RenderDevice &device);

        ~GeometryPass() override;

        void render(RenderTarget &screen, RenderScene &scene, GeometryBuffer &gBuffer) override;

    private:
        RenderDevice &renderDevice;

        ShaderProgram *shaderTextureNormals;
        ShaderProgram *shaderVertexNormals;

        // The default texture buffers with a size of 1x1 pixels into which the color values of the material
        // are stored if the user did not specify a texture in the material.
        TextureBuffer *diffuseDefault;
        TextureBuffer *ambientDefault;
        TextureBuffer *specularDefault;
        TextureBuffer *shininessDefault;
        TextureBuffer *emissiveDefault;

        ShaderProgram *shaderSkybox;
        MeshBuffer *skyboxCube;
        TextureBuffer *skyboxDefault;
    };
}

#endif //MANA_GEOMETRYPASS_HPP
