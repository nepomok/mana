#ifndef MANA_RENDERALLOCATOR_HPP
#define MANA_RENDERALLOCATOR_HPP

#include "engine/render/geometry/mesh.hpp"
#include "engine/render/framebuffer.hpp"
#include "engine/render/textureattributes.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/shaderprogram.hpp"
#include "engine/math/transform.hpp"

namespace mana {
    class RenderAllocator {
    public:
        virtual FrameBuffer *allocateFrameBuffer(int width,
                                                 int height,
                                                 TextureAttributes colorBufferProperties = {}) = 0;

        virtual RenderTexture *allocateTexture(int width,
                                               int height,
                                               TextureAttributes properties = {}) = 0;

        virtual RenderTexture *allocateCubeMapTexture(int width,
                                                      int height,
                                                      TextureAttributes properties = {}) = 0;

        virtual RenderMesh *allocateMesh(const Mesh &mesh) = 0;

        virtual RenderMesh *allocateInstancedMesh(const Mesh &mesh,
                                                           const std::vector<Transform> &offsets = {}) = 0;

        virtual ShaderProgram *allocateShaderProgram(std::string vertexShader, std::string fragmentShader) = 0;
    };
}

#endif //MANA_RENDERALLOCATOR_HPP
