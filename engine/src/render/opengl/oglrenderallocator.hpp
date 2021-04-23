#ifndef MANA_OGLRENDERALLOCATOR_HPP
#define MANA_OGLRENDERALLOCATOR_HPP

#include "engine/render/renderallocator.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderAllocator : public RenderAllocator {
        public:
            FrameBuffer *allocateFrameBuffer(int width, int height, TextureAttributes colorBufferProperties) override;

            RenderTexture *allocateTexture(int width, int height, TextureAttributes properties) override;

            RenderTexture *allocateCubeMapTexture(int width, int height, TextureAttributes properties) override;

            RenderMesh *allocateMesh(const Mesh &mesh) override;

            RenderMesh *allocateInstancedMesh(const Mesh &mesh,
                                              const std::vector<Transform> &offsets) override;

            ShaderProgram *allocateShaderProgram(std::string vertexShader, std::string fragmentShader) override;
        };
    }
}

#endif //MANA_OGLRENDERALLOCATOR_HPP
