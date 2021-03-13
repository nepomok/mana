#ifndef MANA_OGLRENDERAPI_HPP
#define MANA_OGLRENDERAPI_HPP

#include "engine/render/renderapi.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderAPI : public RenderAPI {
        public:
            void render(const RenderCommand &command,
                        const FrameBufferObject &frameBuffer,
                        Vec2i viewportOffset,
                        Vec2i viewportSize,
                        ColorRGBA32 clearColorValue,
                        bool clearColor,
                        bool clearDepth,
                        bool clearStencil,
                        bool multiSample) override;

            void render(const RenderCommand &command,
                        const FrameBufferObject &fb,
                        ColorRGBA32 clearColorValue,
                        bool clearColor,
                        bool clearDepth,
                        bool clearStencil,
                        bool multiSample) override;

            void blitFramebuffer(const FrameBufferObject &fbSource,
                                 const FrameBufferObject &fbTarget,
                                 Vec2i sourceOffset,
                                 Vec2i targetOffset,
                                 Vec2i rect) override;

            void blitFramebuffer(const FrameBufferObject &fbSource, const FrameBufferObject &fbTarget) override;

            void readTextureRGB(const TextureObject &texture, ImageBuffer<ColorRGB24> &output) override;

            void readTextureRGBA(const TextureObject &texture, ImageBuffer<ColorRGBA32> &output) override;

            void writeTextureRGB(const ImageBuffer<ColorRGB24> &input, const TextureObject &tex) override;

            void writeTextureRGBA(const ImageBuffer<ColorRGBA32> &input, const TextureObject &tex) override;

            FrameBufferObject *allocateFrameBuffer(int width,
                                                   int height,
                                                   ColorFormat colorFormat,
                                                   TextureAttributes definition) override;

            FrameBufferObject *allocateFrameBuffer(const TextureObject &texture) override;

            TextureObject *allocateTexture(int width,
                                           int height,
                                           ColorFormat colorFormat,
                                           TextureAttributes definition) override;

            TextureObject *allocateTexture(const ImageBuffer<ColorRGB24> &imageBuffer,
                                           TextureAttributes definition) override;

            TextureObject *allocateTexture(const ImageBuffer<ColorRGBA32> &imageBuffer,
                                           TextureAttributes definition) override;

            TextureObject *allocateTexture(const std::vector<ImageBuffer<ColorRGB24>> &imageBuffers,
                                           TextureAttributes props) override;

            TextureObject *allocateTexture(const std::vector<ImageBuffer<ColorRGBA32>> &imageBuffers,
                                           TextureAttributes props) override;

            MeshObject *allocateMesh(const Mesh &mesh) override;

            MeshObject *allocateMeshInstanced(const Mesh &mesh, std::vector<Transform> offsets) override;

            ShaderProgram *compileShaderProgram(std::string vertexShader, std::string fragmentShader) override;
        };
    }
}

#endif //MANA_OGLRENDERAPI_HPP
