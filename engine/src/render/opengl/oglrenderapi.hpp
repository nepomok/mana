#ifndef MANA_OGLRENDERAPI_HPP
#define MANA_OGLRENDERAPI_HPP

#include "engine/render/renderer.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderAPI : public Renderer {
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
                                 Vec2i sourceRect,
                                 Vec2i targetRect,
                                 TextureFiltering filter) override;

            void blitFramebuffer(const FrameBufferObject &fbSource, const FrameBufferObject &fbTarget) override;

            void readTextureRGB(const RenderTexture &texture, ImageBuffer<ColorRGB24> &output) override;

            void readTextureRGBA(const RenderTexture &texture, ImageBuffer<ColorRGBA32> &output) override;

            void writeTextureRGB(const ImageBuffer<ColorRGB24> &input, const RenderTexture &tex) override;

            void writeTextureRGBA(const ImageBuffer<ColorRGBA32> &input, const RenderTexture &tex) override;

            FrameBufferObject *allocateFrameBuffer(int width,
                                                   int height,
                                                   ColorFormat colorFormat,
                                                   TextureAttributes definition) override;

            FrameBufferObject *allocateFrameBuffer(const RenderTexture &texture) override;

            RenderTexture *allocateTexture(int width,
                                           int height,
                                           ColorFormat colorFormat,
                                           TextureAttributes definition) override;

            RenderTexture *allocateTexture(const ImageBuffer<ColorRGB24> &imageBuffer,
                                           TextureAttributes definition) override;

            RenderTexture *allocateTexture(const ImageBuffer<ColorRGBA32> &imageBuffer,
                                           TextureAttributes definition) override;

            RenderTexture *allocateTexture(const std::vector<ImageBuffer<ColorRGB24>> &imageBuffers,
                                           TextureAttributes props) override;

            RenderTexture *allocateTexture(const std::vector<ImageBuffer<ColorRGBA32>> &imageBuffers,
                                           TextureAttributes props) override;

            RenderMesh *allocateMesh(const Mesh &mesh) override;

            RenderMesh *allocateMeshInstanced(const Mesh &mesh, std::vector<Transform> offsets) override;

            ShaderProgram *compileShaderProgram(std::string vertexShader, std::string fragmentShader) override;
        };
    }
}

#endif //MANA_OGLRENDERAPI_HPP
