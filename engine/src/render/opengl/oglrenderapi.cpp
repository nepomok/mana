#include <string>
#include <set>

#include "engine/math/matrixmath.hpp"

#include "extern/glad.h"

#include "render/opengl/oglrenderapi.hpp"
#include "render/opengl/oglshaderprogram.hpp"
#include "render/opengl/ogltextureobject.hpp"
#include "render/opengl/oglmeshobject.hpp"
#include "render/opengl/ogltypeconverter.hpp"
#include "render/opengl/oglframebufferobject.hpp"
#include "render/opengl/ogluserframebuffer.hpp"

namespace mana {
    namespace opengl {
        void checkGLError(std::string source) {
            GLenum er = glGetError();
            if (er != GL_NO_ERROR) {
                std::string error = source + " GLERROR: ";
                error += std::to_string(er);
                fprintf(stderr, "%s\n", error.c_str());
            }
        }

        GLuint getTextureSlot(int slot) {
            switch (slot) {
                case 0:
                    return GL_TEXTURE0;
                case 1:
                    return GL_TEXTURE1;
                case 2:
                    return GL_TEXTURE2;
                case 3:
                    return GL_TEXTURE3;
                case 4:
                    return GL_TEXTURE4;
                case 5:
                    return GL_TEXTURE5;
                case 6:
                    return GL_TEXTURE6;
                case 7:
                    return GL_TEXTURE7;
                case 8:
                    return GL_TEXTURE8;
                case 9:
                    return GL_TEXTURE9;
            }
            throw std::runtime_error("Unsupported slot");
        }

        /**
         * This function performs the drawing of a render renderCommand to the bound FBO.
         */
        void processCommand(const RenderCommand &command, bool clearColor, bool clearDepth, bool clearStencil) {
            //Check renderCommand
            if (command.directionalLights.size() > 10) {
                throw std::runtime_error("Too many lights in renderCommand.");
            }

            GLbitfield clearMask = 0;
            if (clearColor) {
                clearMask |= GL_COLOR_BUFFER_BIT;
            }

            if (clearDepth) {
                clearMask |= GL_DEPTH_BUFFER_BIT;
            }

            if (clearStencil) {
                clearStencil |= GL_STENCIL_BUFFER_BIT;
            }

            glClear(clearMask);

            Mat4f model, view, projection = MatrixMath::identity();

            //Calculate view matrix
            //The engines move the universe
            view = MatrixMath::rotate(
                    Vec3f(command.camera.transform.rotation.x,
                          command.camera.transform.rotation.y,
                          command.camera.transform.rotation.z));
            view = view * MatrixMath::translate(
                    Vec3f(-command.camera.transform.position.x,
                          -command.camera.transform.position.y,
                          -command.camera.transform.position.z));

            //Calculate projection matrix
            if (command.camera.cameraType == Camera::PERSPECTIVE) {
                Camera::PerspectiveData data = command.camera.cameraData.perspective;
                projection = MatrixMath::perspective(glm::radians(data.fov), data.aspectRatio,
                                                     data.nearClip, data.farClip);
            } else {
                Camera::OrthographicData data = command.camera.cameraData.orthographic;
                projection = MatrixMath::ortho(data.left, data.right, data.bottom, data.top,
                                               data.nearClip, data.farClip);
            }

            std::set<OGLShaderProgram *> shaders;

            for (auto &renderUnit : command.units) {
                model = MatrixMath::translate(
                        Vec3f(renderUnit.transform.position.x,
                              renderUnit.transform.position.y,
                              renderUnit.transform.position.z));
                model = model * MatrixMath::scale(
                        Vec3f(renderUnit.transform.scale.x,
                              renderUnit.transform.scale.y,
                              renderUnit.transform.scale.z));
                model = model * MatrixMath::rotate(Vec3f(renderUnit.transform.rotation.x,
                                                         renderUnit.transform.rotation.y,
                                                         renderUnit.transform.rotation.z));

                //Bind textures
                for (int i = 0; i < renderUnit.textures.size(); i++) {
                    auto *textureObject = renderUnit.textures.at(i);
                    if (textureObject == nullptr) {
                        throw std::runtime_error("nullptr texture");
                    }
                    auto &texture = dynamic_cast<const OGLTextureObject &>(*textureObject);
                    glActiveTexture(getTextureSlot(i));
                    if (texture.cubeMap)
                        glBindTexture(GL_TEXTURE_CUBE_MAP, texture.handle);
                    else
                        glBindTexture(GL_TEXTURE_2D, texture.handle);
                }

                //Bind shader program
                ShaderProgram *sp = renderUnit.shader;
                if (sp == nullptr) {
                    throw std::runtime_error("nullptr shaderprogram");
                }
                auto &shader = dynamic_cast<OGLShaderProgram &>(*sp);
                shader.activate();

                shaders.insert(&shader);

                //Assign shader uniforms
                //Matrices
                shader.setMat4("MANA_M", model);
                shader.setMat4("MANA_V", view);
                shader.setMat4("MANA_P", projection);
                shader.setMat4("MANA_MVP", projection * view * model);

                //Lights
                int i = 0;
                for (auto &light : command.pointLights) {
                    std::string name = "MANA_LIGHTS_POINT[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "position", light.transform.position);
                    shader.setFloat(name + "constant", light.constant);
                    shader.setFloat(name + "linear", light.linear);
                    shader.setFloat(name + "quadratic", light.quadratic);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }
                i = 0;
                for (auto &light : command.spotLights) {
                    std::string name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "position", light.transform.position);
                    shader.setVec3(name + "direction", light.direction);
                    shader.setFloat(name + "cutOff",
                                    glm::cos(glm::radians(light.cutOff)));
                    shader.setFloat(name + "outerCutOff",
                                    glm::cos(glm::radians(light.outerCutOff)));
                    shader.setFloat(name + "constant", light.constant);
                    shader.setFloat(name + "linear", light.linear);
                    shader.setFloat(name + "quadratic", light.quadratic);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }
                i = 0;
                for (auto &light : command.directionalLights) {
                    std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "direction", light.direction);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }

                shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", command.directionalLights.size());
                shader.setInt("MANA_LIGHT_COUNT_POINT", command.pointLights.size());
                shader.setInt("MANA_LIGHT_COUNT_SPOT", command.spotLights.size());

                //Viewport
                shader.setVec3("MANA_VIEWPOS", command.camera.transform.position);

                //Setup per model depth, stencil, culling and blend states
                if (renderUnit.enableDepthTest) {
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(OGLTypeConverter::convert(renderUnit.depthTestMode));
                    if (renderUnit.depthTestWrite)
                        glDepthMask(GL_TRUE);
                    else
                        glDepthMask(GL_FALSE);
                } else {
                    glDisable(GL_DEPTH_TEST);
                }

                if (renderUnit.enableStencilTest) {
                    glEnable(GL_STENCIL_TEST);
                    glStencilMask(OGLTypeConverter::convertPrimitive(renderUnit.stencilTestMask));
                    glStencilFunc(OGLTypeConverter::convert(renderUnit.stencilMode),
                                  OGLTypeConverter::convertPrimitive(renderUnit.stencilReference),
                                  OGLTypeConverter::convertPrimitive(renderUnit.stencilFunctionMask));
                    glStencilOp(OGLTypeConverter::convert(renderUnit.stencilFail),
                                OGLTypeConverter::convert(renderUnit.stencilDepthFail),
                                OGLTypeConverter::convert(renderUnit.stencilPass));
                } else {
                    glDisable(GL_STENCIL_TEST);
                }

                if (renderUnit.enableFaceCulling) {
                    glEnable(GL_CULL_FACE);
                    glCullFace(OGLTypeConverter::convert(renderUnit.faceCullMode));
                    if (renderUnit.faceCullClockwiseWinding)
                        glFrontFace(GL_CW);
                    else
                        glFrontFace(GL_CCW);
                } else {
                    glDisable(GL_CULL_FACE);
                }

                if (renderUnit.enableBlending) {
                    glEnable(GL_BLEND);
                    glBlendFunc(OGLTypeConverter::convert(renderUnit.blendSourceMode),
                                OGLTypeConverter::convert(renderUnit.blendDestinationMode));
                } else {
                    glDisable(GL_BLEND);
                }

                //Bind VAOs and draw.
                for (auto meshPtr : renderUnit.meshData) {
                    if (meshPtr == nullptr) {
                        throw std::runtime_error("nullptr meshData");
                    }
                    auto &mesh = dynamic_cast<const OGLMeshObject &>(*meshPtr);

                    glBindVertexArray(mesh.VAO);
                    if (mesh.indexed) {
                        if (mesh.instanced)
                            glDrawElementsInstanced(GL_TRIANGLES, mesh.elementCount * 3, GL_UNSIGNED_INT, 0,
                                                    mesh.instanceCount);
                        else
                            glDrawElements(GL_TRIANGLES, mesh.elementCount * 3, GL_UNSIGNED_INT, 0);
                    } else {
                        if (mesh.instanced)
                            glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.elementCount * 3, mesh.instanceCount);
                        else
                            glDrawArrays(GL_TRIANGLES, 0, mesh.elementCount * 3);
                    }
                    glBindVertexArray(0);
                }

                //Unbind textures
                for (int y = 0; y < 10; y++) {
                    glActiveTexture(getTextureSlot(y));
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                }
            }
        }

        void OGLRenderAPI::render(const RenderCommand &command,
                                  const FrameBufferObject &frameBuffer,
                                  Vec2i viewportOffset,
                                  Vec2i viewportSize,
                                  ColorRGBA32 clearColorValue,
                                  bool clearColor,
                                  bool clearDepth,
                                  bool clearStencil,
                                  bool multiSample) {
            glClearColor((float) clearColorValue.r / (float) 255,
                         (float) clearColorValue.g / (float) 255,
                         (float) clearColorValue.b / (float) 255,
                         (float) clearColorValue.a / (float) 255);

            if (multiSample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);

            auto &fb = dynamic_cast<const OGLFrameBufferObject &>(frameBuffer);

            GLint vpData[4];
            glGetIntegerv(GL_VIEWPORT, vpData);

            glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);

            glBindFramebuffer(GL_FRAMEBUFFER, fb.getFBO());
            processCommand(command, clearColor, clearDepth, clearStencil);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(vpData[0], vpData[1], vpData[2], vpData[3]);

            checkGLError("OGLRenderAPI::render");
        }

        void OGLRenderAPI::render(const RenderCommand &command,
                                  const FrameBufferObject &frameBuffer,
                                  ColorRGBA32 clearColorValue,
                                  bool clearColor,
                                  bool clearDepth,
                                  bool clearStencil,
                                  bool multiSample) {
            auto &fb = dynamic_cast<const OGLFrameBufferObject &>(frameBuffer);
            render(command,
                   frameBuffer,
                   Vec2i(0),
                   fb.getSize(),
                   clearColorValue,
                   clearColor,
                   clearDepth,
                   clearStencil,
                   multiSample);
        }

        void OGLRenderAPI::blitFramebuffer(const FrameBufferObject &fbSource,
                                           const FrameBufferObject &fbTarget,
                                           Vec2i sourceOffset,
                                           Vec2i targetOffset,
                                           Vec2i sourceRect,
                                           Vec2i targetRect,
                                           TextureFiltering filter) {
            if (sourceRect.x < 0 || sourceRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (sourceOffset.x < 0 || sourceOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }
            if (targetRect.x < 0 || targetRect.y < 0) {
                throw std::runtime_error("Rect cannot be negative");
            }
            if (targetOffset.x < 0 || targetOffset.y < 0) {
                throw std::runtime_error("Offset cannot be negative");
            }

            auto &fbS = dynamic_cast<const OGLFrameBufferObject &>(fbSource);
            auto &fbT = dynamic_cast<const OGLFrameBufferObject &>(fbTarget);

            Vec2i sourceSize = fbS.getSize();
            if (sourceSize.x < sourceRect.x + sourceOffset.x || sourceSize.y < sourceRect.y + sourceOffset.y)
                throw std::runtime_error("Blit rect out of bounds for source framebuffer");

            Vec2i targetSize = fbT.getSize();
            if (targetSize.x < targetRect.x + targetOffset.x || targetSize.y < targetRect.y + targetOffset.y)
                throw std::runtime_error("Blit rect out of bounds for target framebuffer.");

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbS.getFBO());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbT.getFBO());
            glBlitFramebuffer(sourceOffset.x,
                              sourceOffset.y,
                              sourceRect.x,
                              sourceRect.y,
                              targetOffset.x,
                              targetOffset.y,
                              targetRect.x,
                              targetRect.y,
                              GL_COLOR_BUFFER_BIT,
                              OGLTypeConverter::convert(filter));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("OGLRenderAPI::blitFramebuffer");
        }

        void OGLRenderAPI::blitFramebuffer(const FrameBufferObject &fbSource, const FrameBufferObject &fbTarget) {
            auto &fbS = dynamic_cast<const OGLFrameBufferObject &>(fbSource);
            auto &fbT = dynamic_cast<const OGLFrameBufferObject &>(fbTarget);
            blitFramebuffer(fbSource,
                            fbTarget,
                            Vec2i(0),
                            Vec2i(0),
                            fbS.getSize(),
                            fbT.getSize(),
                            TextureFiltering::NEAREST);
        }

        void OGLRenderAPI::readTextureRGB(const TextureObject &texture, ImageBuffer<ColorRGB24> &output) {
            auto &tex = dynamic_cast<const OGLTextureObject &>(texture);
            output = ImageBuffer<ColorRGB24>(tex.width, tex.height);
            glBindTexture(GL_TEXTURE_2D, tex.handle);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, output.buffer.getData());
            glBindTexture(GL_TEXTURE_2D, 0);
            checkGLError("OGLRenderAPI::readTextureRGB");
        }

        void OGLRenderAPI::readTextureRGBA(const TextureObject &texture, ImageBuffer<ColorRGBA32> &output) {
            auto &tex = dynamic_cast<const OGLTextureObject &>(texture);
            output = ImageBuffer<ColorRGBA32>(tex.width, tex.height);
            glBindTexture(GL_TEXTURE_2D, tex.handle);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, output.buffer.getData());
            glBindTexture(GL_TEXTURE_2D, 0);
            checkGLError("OGLRenderAPI::readTextureRGBA");
        }

        void OGLRenderAPI::writeTextureRGB(const ImageBuffer<ColorRGB24> &input, const TextureObject &tex) {
            auto &texture = dynamic_cast<const OGLTextureObject &>(tex);
            if (input.getWidth() != texture.width || input.getHeight() != texture.height) {
                throw std::runtime_error("Attempted to write input buffer with non matching size");
            }
            glBindTexture(GL_TEXTURE_2D, texture.handle);
            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            0,
                            0,
                            texture.width,
                            texture.height,
                            GL_RGB,
                            GL_UNSIGNED_BYTE,
                            input.buffer.getData());
            glBindTexture(GL_TEXTURE_2D, 0);
            checkGLError("OGLRenderAPI::writeTextureRGB");
        }

        void OGLRenderAPI::writeTextureRGBA(const ImageBuffer<ColorRGBA32> &input, const TextureObject &tex) {
            auto &texture = dynamic_cast<const OGLTextureObject &>(tex);
            if (input.getWidth() != texture.width || input.getHeight() != texture.height) {
                throw std::runtime_error("Attempted to write input buffer with non matching size");
            }
            glBindTexture(GL_TEXTURE_2D, texture.handle);
            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            0,
                            0,
                            texture.width,
                            texture.height,
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            input.buffer.getData());
            glBindTexture(GL_TEXTURE_2D, 0);
            checkGLError("OGLRenderAPI::writeTextureRGBA");
        }

        FrameBufferObject *OGLRenderAPI::allocateFrameBuffer(int width,
                                                             int height,
                                                             ColorFormat colorFormat,
                                                             TextureAttributes definition) {
            auto *ret = new OGLUserFrameBuffer(width, height);

            glGenFramebuffers(1, &ret->FBO);
            glBindFramebuffer(GL_FRAMEBUFFER, ret->FBO);

            glGenTextures(1, &ret->colorBuffer);
            glBindTexture(GL_TEXTURE_2D, ret->colorBuffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(definition.texWrapping));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(definition.texWrapping));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMin));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMag));

            switch (colorFormat) {
                case RGB24:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                    break;
                case RGBA32:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                    break;
            }

            if (definition.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(definition.mipmapFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(definition.texFilterMag));
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ret->colorBuffer, 0);

            glGenRenderbuffers(1, &ret->renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, ret->renderBuffer);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ret->width, ret->height);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                      ret->renderBuffer);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Failed to setup framebuffer");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("OGLRenderAPI::allocateFrameBuffer");

            return ret;
        }

        FrameBufferObject *OGLRenderAPI::allocateFrameBuffer(const TextureObject &texture) {
            auto &tex = dynamic_cast<const OGLTextureObject &>(texture);

            auto *ret = new OGLUserFrameBuffer(tex.width, tex.height);

            ret->userTexture = true;
            ret->colorBuffer = tex.handle;

            glGenFramebuffers(1, &ret->FBO);
            glBindFramebuffer(GL_FRAMEBUFFER, ret->FBO);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ret->colorBuffer, 0);

            glGenRenderbuffers(1, &ret->renderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, ret->renderBuffer);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ret->width, ret->height);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                      ret->renderBuffer);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Failed to setup framebuffer");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            checkGLError("OGLRenderAPI::allocateFrameBuffer");

            return ret;
        }

        TextureObject *OGLRenderAPI::allocateTexture(int width,
                                                     int height,
                                                     ColorFormat colorFormat,
                                                     TextureAttributes definition) {
            auto *ret = new OGLTextureObject(width, height);

            glGenTextures(1, &ret->handle);
            glBindTexture(GL_TEXTURE_2D, ret->handle);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(definition.texWrapping));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(definition.texWrapping));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMin));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMag));

            switch (colorFormat) {
                case RGB24:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                    break;
                case RGBA32:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                    break;
            }

            if (definition.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(definition.mipmapFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(definition.texFilterMag));
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            checkGLError("OGLRenderAPI::allocateTexture");

            return ret;
        }

        TextureObject *OGLRenderAPI::allocateTexture(const ImageBuffer<ColorRGB24> &imageBuffer,
                                                     TextureAttributes definition) {
            auto *ret = new OGLTextureObject(imageBuffer.getWidth(), imageBuffer.getHeight());

            glGenTextures(1, &ret->handle);
            glBindTexture(GL_TEXTURE_2D, ret->handle);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(definition.texWrapping));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(definition.texWrapping));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMin));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMag));

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageBuffer.getWidth(), imageBuffer.getHeight(), 0, GL_RGB,
                         GL_UNSIGNED_BYTE, imageBuffer.buffer.getData());

            if (definition.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(definition.mipmapFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(definition.texFilterMag));
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            checkGLError("OGLRenderAPI::allocateTexture");

            return ret;
        }

        TextureObject *OGLRenderAPI::allocateTexture(const ImageBuffer<ColorRGBA32> &imageBuffer,
                                                     TextureAttributes definition) {
            auto *ret = new OGLTextureObject(imageBuffer.getWidth(), imageBuffer.getHeight());

            glGenTextures(1, &ret->handle);
            glBindTexture(GL_TEXTURE_2D, ret->handle);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(definition.texWrapping));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(definition.texWrapping));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMin));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(definition.texFilterMag));

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageBuffer.getWidth(), imageBuffer.getHeight(), 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, imageBuffer.buffer.getData());

            if (definition.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(definition.mipmapFilter));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(definition.texFilterMag));
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            checkGLError("OGLRenderAPI::allocateTexture");

            return ret;
        }

        TextureObject *OGLRenderAPI::allocateTexture(const std::vector<ImageBuffer<ColorRGB24>> &imageBuffers,
                                                     TextureAttributes props) {
            auto *ret = new OGLTextureObject(imageBuffers.at(0).getWidth(), imageBuffers.at(0).getHeight());

            ret->cubeMap = true;

            glGenTextures(1, &ret->handle);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ret->handle);

            for (unsigned int i = 0; i < imageBuffers.size(); i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB,
                             imageBuffers.at(i).getWidth(),
                             imageBuffers.at(i).getHeight(),
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             imageBuffers.at(i).buffer.getData());
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(props.texWrapping));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(props.texWrapping));

            glTexParameteri(GL_TEXTURE_CUBE_MAP,
                            GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(props.texFilterMin));
            glTexParameteri(GL_TEXTURE_CUBE_MAP,
                            GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(props.texFilterMag));

            if (props.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                glTexParameteri(GL_TEXTURE_CUBE_MAP,
                                GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(props.mipmapFilter));
                glTexParameteri(GL_TEXTURE_CUBE_MAP,
                                GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(props.texFilterMag));
            }

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            checkGLError("OGLRenderAPI::allocateTexture");

            return ret;
        }

        TextureObject *OGLRenderAPI::allocateTexture(const std::vector<ImageBuffer<ColorRGBA32>> &imageBuffers,
                                                     TextureAttributes props) {
            auto *ret = new OGLTextureObject(imageBuffers.at(0).getWidth(), imageBuffers.at(0).getHeight());

            ret->cubeMap = true;

            glGenTextures(1, &ret->handle);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ret->handle);

            for (unsigned int i = 0; i < imageBuffers.size(); i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB,
                             imageBuffers.at(i).getWidth(),
                             imageBuffers.at(i).getHeight(),
                             0,
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             imageBuffers.at(i).buffer.getData());
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(props.texWrapping));
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(props.texWrapping));

            glTexParameteri(GL_TEXTURE_CUBE_MAP,
                            GL_TEXTURE_MIN_FILTER,
                            OGLTypeConverter::convert(props.texFilterMin));
            glTexParameteri(GL_TEXTURE_CUBE_MAP,
                            GL_TEXTURE_MAG_FILTER,
                            OGLTypeConverter::convert(props.texFilterMag));

            if (props.generateMipmap) {
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                glTexParameteri(GL_TEXTURE_CUBE_MAP,
                                GL_TEXTURE_MIN_FILTER,
                                OGLTypeConverter::convert(props.mipmapFilter));
                glTexParameteri(GL_TEXTURE_CUBE_MAP,
                                GL_TEXTURE_MAG_FILTER,
                                OGLTypeConverter::convert(props.texFilterMag));
            }

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            checkGLError("OGLRenderAPI::allocateTexture");

            return ret;
        }

        MeshObject *OGLRenderAPI::allocateMesh(const Mesh &mesh) {
            if (mesh.primitive != TRI) {
                throw std::runtime_error("Unsupported primitive");
            }

            auto *ret = new OGLMeshObject();

            ret->indexed = mesh.indexed;
            ret->instanced = false;

            glm::mat4 offset(1);

            if (mesh.indexed) {
                ret->elementCount = mesh.indices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->EBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, Vertex::BYTES * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.indices.size(), mesh.indices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &offset, GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            } else {
                ret->elementCount = mesh.vertices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, Vertex::BYTES * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &offset, GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            checkGLError("OGLRenderAPI::allocateMesh");

            return ret;
        }

        MeshObject *OGLRenderAPI::allocateMeshInstanced(const Mesh &mesh,
                                                        std::vector<Transform> offsets) {
            if (mesh.primitive != TRI) {
                throw std::runtime_error("Unsupported primitive");
            }

            auto *ret = new OGLMeshObject();

            ret->indexed = mesh.indexed;

            ret->instanced = true;
            ret->instanceCount = offsets.size();

            glm::mat4 off[offsets.size()];

            for (int i = 0; i < offsets.size(); i++) {
                off[i] = OGLTypeConverter::convert(MatrixMath::translate(offsets.at(i).position)
                                                   * MatrixMath::scale(offsets.at(i).scale)
                                                   * MatrixMath::rotate(offsets.at(i).rotation)
                );
            }

            if (mesh.indexed) {
                ret->elementCount = mesh.indices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->EBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, Vertex::BYTES * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.indices.size(), mesh.indices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * offsets.size(), &off[0], GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            } else {
                ret->elementCount = mesh.vertices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, Vertex::BYTES * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::BYTES, (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * offsets.size(), &off[0], GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(glm::vec4)));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (2 * sizeof(glm::vec4)));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                      (void *) (3 * sizeof(glm::vec4)));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            checkGLError("OGLRenderAPI::allocateMeshInstanced");

            return ret;
        }

        ShaderProgram *OGLRenderAPI::compileShaderProgram(std::string vertexShader, std::string fragmentShader) {
            ShaderProgram *ret = new OGLShaderProgram(vertexShader, fragmentShader);
            checkGLError("OGLRenderAPI::compileShaderProgram");
            return ret;
        }
    }
}