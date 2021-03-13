#ifndef MANA_RENDERAPI_HPP
#define MANA_RENDERAPI_HPP

#include "engine/io/imagebuffer.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/framebufferobject.hpp"
#include "engine/render/meshobject.hpp"
#include "engine/render/textureobject.hpp"
#include "engine/render/textureattributes.hpp"

namespace mana {
    /**
     * The (preferably stateless) graphics api specific logic abstraction.
     */
    class RenderAPI {
    public:
        /**
         * -----------------
         * --- Rendering ---
         * -----------------
         * */

        /**
         * Render the renderCommand to the framebuffer.
         * The renderCommand is rendered using the specified offset and size.
         *
         * @param command The renderCommand to render.
         * @param fb The framebuffer to render the renderCommand to.
         * @param viewportOffset The offset of the viewport into the target framebuffer.
         * @param viewportSize The size of the viewport in the target framebuffer.
         * @param clearColorValue The clear color.
         * @param clearColor Set to true to clear the color data in the framebuffer before rendering.
         * @param clearDepth Set to true to clear the depth data in the framebuffer before rendering.
         * @param clearStencil Set to true to clear the stencil data in the framebuffer before rendering.
         * @param multiSample Set to true to enable multi sampling for this render operation.
         */
        virtual void render(const RenderCommand &command,
                            const FrameBufferObject &fb,
                            Vec2i viewportOffset,
                            Vec2i viewportSize,
                            ColorRGBA32 clearColorValue,
                            bool clearColor,
                            bool clearDepth,
                            bool clearStencil,
                            bool multiSample) = 0;

        /**
         * Render the renderCommand to the framebuffer.
         * The renderCommand is rendered with a offset of 0 and the size of the framebuffer.
         *
         * @param command
         * @param fb
         * @param clearColorValue
         * @param clearColor
         * @param clearDepth
         * @param clearStencil
         * @param multiSample
         */
        virtual void render(const RenderCommand &command,
                            const FrameBufferObject &fb,
                            ColorRGBA32 clearColorValue = ColorRGBA32(0),
                            bool clearColor = true,
                            bool clearDepth = true,
                            bool clearStencil = true,
                            bool multiSample = true) = 0;

        /**
         * Perform a hardware blit of the two framebuffers.
         *
         * @param fbSource The source framebuffer to sample from.
         * @param fbTarget The target framebuffer to blit into.
         * @param sourceOffset
         * @param targetOffset
         * @param rect
         * @param targetRect
         */
        virtual void blitFramebuffer(const FrameBufferObject &fbSource,
                                     const FrameBufferObject &fbTarget,
                                     Vec2i sourceOffset,
                                     Vec2i targetOffset,
                                     Vec2i rect) = 0;

        /**
         * Perform a hardware blit of the two framebuffers.
         *
         * @param fbSource
         * @param fbTarget
         */
        virtual void blitFramebuffer(const FrameBufferObject &fbSource, const FrameBufferObject &fbTarget) = 0;

        /**
         * --------------------------------
         * --- I/O Between host and api ---
         * --------------------------------
         * */

        /**
         * Copy image data from a texture handle into host memory.
         *
         * @param texture
         * @param output
         */
        virtual void readTextureRGB(const TextureObject &texture, ImageBuffer<ColorRGB24> &output) = 0;

        /**
         *
         * @param texture
         * @param output
         */
        virtual void readTextureRGBA(const TextureObject &texture, ImageBuffer<ColorRGBA32> &output) = 0;

        /**
         * Copy image data in a host buffer to a target texture.
         *
         * @param input
         * @param tex
         */
        virtual void writeTextureRGB(const ImageBuffer<ColorRGB24> &input, const TextureObject &tex) = 0;

        /**
         *
         * @param input
         * @param tex
         */
        virtual void writeTextureRGBA(const ImageBuffer<ColorRGBA32> &input, const TextureObject &tex) = 0;

        /**
         * ---------------------------
         * --- Resource Management ---
         * ---------------------------
         */

        /**
         * Allocate a framebuffer.
         *
         * Can be used for example to perform multi pass rendering.
         *
         * @param width
         * @param height
         * @param colorFormat
         * @param props
         * @return
         */
        virtual FrameBufferObject *allocateFrameBuffer(int width,
                                                       int height,
                                                       ColorFormat colorFormat,
                                                       TextureAttributes props) = 0;

        /**
         * Allocate a framebuffer which uses the passed texture object as texture.
         *
         * Changes to the returned framebuffer are reflected in the passed
         * texture argument.
         *
         * Note that when deleting the passed texture object the returned frame buffer becomes invalid,
         * and the returned framebuffer object destructor will not delete the passed texture object.
         *
         * @param texture The texture to bindContext to the returned framebuffer.
         * @return The frame buffer handle.
         */
        virtual FrameBufferObject *allocateFrameBuffer(const TextureObject &texture) = 0;

        /**
         * Allocate a texture object.
         *
         * The returned texture handle is only valid for the currently bound context.
         *
         * @param width
         * @param height
         * @param colorFormat
         * @param props
         * @return
         */
        virtual TextureObject *allocateTexture(int width,
                                               int height,
                                               ColorFormat colorFormat,
                                               TextureAttributes props) = 0;

        /**
         *
         * @param imageBuffer
         * @param props
         * @return
         */
        virtual TextureObject *allocateTexture(const ImageBuffer<ColorRGB24> &imageBuffer,
                                               TextureAttributes props) = 0;

        /**
         * Allocate a cubemap texture from 6 image buffers.
         * @param imageBuffer
         * @param props
         * @return
         */
        virtual TextureObject *allocateTexture(const ImageBuffer<ColorRGBA32> &imageBuffer,
                                               TextureAttributes props) = 0;

        /**
        *
        * @param imageBuffer
        * @param props
        * @return
        */
        virtual TextureObject *allocateTexture(const std::vector<ImageBuffer<ColorRGB24>> &imageBuffer,
                                               TextureAttributes props) = 0;

        /**
         *
         * @param imageBuffer
         * @param props
         * @return
         */
        virtual TextureObject *allocateTexture(const std::vector<ImageBuffer<ColorRGBA32>> &imageBuffer,
                                               TextureAttributes props) = 0;

        /**
         * Allocate a mesh object copying the data in the mesh argument.
         * The returned mesh handle is only valid for the currently bound context.
         *
         * @param mesh The mesh data to copy into the api mesh.
         * @return A handle to the api dependent mesh.
         */
        virtual MeshObject *allocateMesh(const Mesh &mesh) = 0;

        /**
         * Allocate a mesh object copying the data in the mesh argument.
         * The returned mesh handle is only valid for the currently bound context.
         *
         * When the returned mesh handle is rendered it is rendered in instanced mode.
         *
         * In instanced mode the mesh is rendered offset.size() times with the passed offset transforms relative to the RenderUnit transform.
         *
         * Note that the offsets cannot change after allocating the mesh handle.
         *
         * @param mesh The mesh data to copy into the api mesh.
         * @param offsets
         * @return A handle to the api dependent mesh.
         */
        virtual MeshObject *allocateMeshInstanced(const Mesh &mesh, std::vector<Transform> offsets) = 0;

        /**
         *
         * @param vertexShader
         * @param fragmentShader
         * @return
         */
        virtual ShaderProgram *compileShaderProgram(std::string vertexShader, std::string fragmentShader) = 0;
    };
}

#endif //MANA_RENDERAPI_HPP
