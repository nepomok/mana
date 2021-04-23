#ifndef MANA_RENDERALLOCATOR_HPP
#define MANA_RENDERALLOCATOR_HPP

#include "engine/render/geometry/mesh.hpp"
#include "engine/render/framebuffer.hpp"
#include "engine/render/textureattributes.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/rendermeshinstanced.hpp"

namespace mana {
    class RenderAllocator {
    public:
        virtual FrameBuffer *allocateFrameBuffer(int width,
                                                 int height,
                                                 ColorFormat colorFormat = ColorFormat::RGBA32,
                                                 TextureAttributes colorProperties = {}) = 0;

        virtual RenderTexture *allocateTexture(int width,
                                               int height,
                                               ColorFormat colorFormat = ColorFormat::RGBA32,
                                               TextureAttributes properties = {}) = 0;

        virtual RenderCubeMapTexture *allocateCubeMapTexture(int width,
                                                             int height,
                                                             ColorFormat colorFormat = ColorFormat::RGBA32,
                                                             TextureAttributes properties = {}) = 0;

        virtual RenderMesh *allocateMesh(const Mesh &mesh) = 0;

        virtual RenderInstancedMesh *allocateInstancedMesh(const Mesh &mesh) = 0;

        virtual ShaderProgram *allocateShaderProgram(std::string vertexShader, std::string fragmentShader) = 0;
    };
}

#endif //MANA_RENDERALLOCATOR_HPP
