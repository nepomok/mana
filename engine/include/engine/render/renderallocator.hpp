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

#ifndef MANA_RENDERALLOCATOR_HPP
#define MANA_RENDERALLOCATOR_HPP

#include "engine/render/rendertarget.hpp"
#include "engine/render/texturebuffer.hpp"
#include "engine/render/meshbuffer.hpp"
#include "engine/render/shaderprogram.hpp"
#include "engine/render/shadersource.hpp"

namespace engine {
    class RenderAllocator {
    public:
        virtual RenderTarget *createRenderTarget(Vec2i size, int samples) = 0;

        virtual TextureBuffer *createTextureBuffer(TextureBuffer::Attributes attributes) = 0;

        /**
         * Create a mesh buffer instance for the given mesh.
         *
         * The mesh buffer vertex attributes required in vertex shaders which draw the mesh buffer are bound as follows:
         *
         * GLSL:
         *  layout (location = 0) in vec3 position;
         *  layout (location = 1) in vec3 normal;
         *  layout (location = 2) in vec2 uv;
         *  layout (location = 3) in vec3 tangent;
         *  layout (location = 4) in vec3 bitangent;
         *  layout (location = 5) in vec4 instanceRow0;
         *  layout (location = 6) in vec4 instanceRow1;
         *  layout (location = 7) in vec4 instanceRow2;
         *  layout (location = 8) in vec4 instanceRow3;
         *
         * HLSL:
         *  struct VS_INPUT
         *  {
         *      float3 position : POSITION0;
         *      float3 normal : NORMAL;
         *      float2 uv : TEXCOORD0;
         *      float3 tangent: TANGENT;
         *      float3 bitangent: BINORMAL;
         *      float4 instanceRow0 : POSITION1;
         *      float4 instanceRow1 : POSITION2;
         *      float4 instanceRow2 : POSITION3;
         *      float4 instanceRow3 : POSITION4;
         *  };
         *
         * @param mesh
         * @return
         */
        virtual MeshBuffer *createMeshBuffer(const Mesh &mesh) = 0;

        virtual MeshBuffer *createInstancedMeshBuffer(const Mesh &mesh, const std::vector<Transform> &offsets) = 0;

        /**
         * Create a shader program instance for the given shader sources.
         *
         * The implementation may cross compile the source to a different language using the ShaderCompiler interface.
         *
         * @param vertexShader
         * @param fragmentShader
         * @return
         */
        virtual ShaderProgram *createShaderProgram(const ShaderSource &vertexShader,
                                                   const ShaderSource &fragmentShader) = 0;

        virtual ShaderProgram *createShaderProgram(const ShaderSource &vertexShader,
                                                   const ShaderSource &geometryShader,
                                                   const ShaderSource &fragmentShader) = 0;
    };
}

#endif //MANA_RENDERALLOCATOR_HPP
