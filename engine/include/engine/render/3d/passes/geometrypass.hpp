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

namespace engine {
    class GeometryPass : public RenderPass {
    public:
        static const char *DEPTH;
        static const char *POSITION;
        static const char *NORMAL;
        static const char *DIFFUSE;
        static const char *AMBIENT;
        static const char *SPECULAR;
        static const char *SHININESS;
        static const char *EMISSIVE;
        static const char *ID;

        explicit GeometryPass(RenderDevice &device);

        ~GeometryPass() override;

        void prepareBuffer(GeometryBuffer &gBuffer) override;

        void render(GeometryBuffer &gBuffer, Scene &scene) override;

    private:
        RenderDevice &renderDevice;

        std::unique_ptr<ShaderProgram> shaderTextureNormals;
        std::unique_ptr<ShaderProgram> shaderVertexNormals;

        // The default texture buffers with a size of 1x1 pixels into which the color values of the material
        // are stored if the user did not specify a texture in the material.
        std::unique_ptr<TextureBuffer> diffuseDefault;
        std::unique_ptr<TextureBuffer> ambientDefault;
        std::unique_ptr<TextureBuffer> specularDefault;
        std::unique_ptr<TextureBuffer> shininessDefault;
        std::unique_ptr<TextureBuffer> emissiveDefault;
    };
}

#endif //MANA_GEOMETRYPASS_HPP
