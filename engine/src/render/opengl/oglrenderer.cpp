/**
 *  Mana - 3D Engine
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

#include <string>
#include <set>

#include "engine/math/matrixmath.hpp"
#include "engine/math/rotation.hpp"

#include "render/opengl/oglrenderer.hpp"

#include "render/opengl/oglshaderprogram.hpp"
#include "render/opengl/oglrendertexture.hpp"
#include "render/opengl/oglmeshobject.hpp"
#include "render/opengl/oglframebuffer.hpp"

#include "render/opengl/oglcheckerror.hpp"
#include "render/opengl/ogltypeconverter.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
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
                default:
                    throw std::runtime_error("Maximum 10 texture slots");
            }
        }

        /**
         * This function performs the drawing of a render scene to the bound FBO.
         */
        void renderScene(const RenderScene &scene, bool clearColor, bool clearDepth, bool clearStencil) {
            if (scene.directionalLights.size() > 10) {
                throw std::runtime_error("Too many lights in scene.");
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

            Mat4f model, view, projection;

            view = scene.camera->view();
            projection = scene.camera->projection();

            std::set<OGLShaderProgram *> shaders;

            for (auto &renderCommand : scene.commands) {
                model = MatrixMath::translate(renderCommand.transform.position);
                model = model * MatrixMath::scale(renderCommand.transform.scale);
                model = model * MatrixMath::rotate(renderCommand.transform.rotation);

                //Bind textures
                for (int i = 0; i < renderCommand.textureObjects.size(); i++) {
                    auto *textureObject = renderCommand.textureObjects.at(i);
                    if (textureObject == nullptr) {
                        throw std::runtime_error("nullptr texture");
                    }
                    auto &texture = dynamic_cast<const OGLRenderTexture &>(*textureObject);
                    glActiveTexture(getTextureSlot(i));
                    glBindTexture(OGLTypeConverter::convert(texture.attributes.textureType), texture.handle);
                }

                //Bind shader program
                ShaderProgram *sp = renderCommand.shader;
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
                for (auto &light : scene.pointLights) {
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
                for (auto &light : scene.spotLights) {
                    std::string name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "position", light.transform.position);
                    shader.setVec3(name + "direction", light.direction);
                    shader.setFloat(name + "cutOff", cosf(degreesToRadians(light.cutOff)));
                    shader.setFloat(name + "outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                    shader.setFloat(name + "constant", light.constant);
                    shader.setFloat(name + "linear", light.linear);
                    shader.setFloat(name + "quadratic", light.quadratic);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }
                i = 0;
                for (auto &light : scene.directionalLights) {
                    std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "direction", light.direction);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }

                shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", scene.directionalLights.size());
                shader.setInt("MANA_LIGHT_COUNT_POINT", scene.pointLights.size());
                shader.setInt("MANA_LIGHT_COUNT_SPOT", scene.spotLights.size());

                shader.setVec3("MANA_VIEWPOS", scene.camera->transform.position);

                //Setup per model depth, stencil, culling and blend states
                if (renderCommand.enableDepthTest) {
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(OGLTypeConverter::convert(renderCommand.depthTestMode));
                    if (renderCommand.depthTestWrite)
                        glDepthMask(GL_TRUE);
                    else
                        glDepthMask(GL_FALSE);
                } else {
                    glDisable(GL_DEPTH_TEST);
                }

                if (renderCommand.enableStencilTest) {
                    glEnable(GL_STENCIL_TEST);
                    glStencilMask(OGLTypeConverter::convertPrimitive(renderCommand.stencilTestMask));
                    glStencilFunc(OGLTypeConverter::convert(renderCommand.stencilMode),
                                  OGLTypeConverter::convertPrimitive(renderCommand.stencilReference),
                                  OGLTypeConverter::convertPrimitive(renderCommand.stencilFunctionMask));
                    glStencilOp(OGLTypeConverter::convert(renderCommand.stencilFail),
                                OGLTypeConverter::convert(renderCommand.stencilDepthFail),
                                OGLTypeConverter::convert(renderCommand.stencilPass));
                } else {
                    glDisable(GL_STENCIL_TEST);
                }

                if (renderCommand.enableFaceCulling) {
                    glEnable(GL_CULL_FACE);
                    glCullFace(OGLTypeConverter::convert(renderCommand.faceCullMode));
                    if (renderCommand.faceCullClockwiseWinding)
                        glFrontFace(GL_CW);
                    else
                        glFrontFace(GL_CCW);
                } else {
                    glDisable(GL_CULL_FACE);
                }

                if (renderCommand.enableBlending) {
                    glEnable(GL_BLEND);
                    glBlendFunc(OGLTypeConverter::convert(renderCommand.blendSourceMode),
                                OGLTypeConverter::convert(renderCommand.blendDestinationMode));
                } else {
                    glDisable(GL_BLEND);
                }

                //Bind VAOs and draw.
                for (auto meshPtr : renderCommand.meshObjects) {
                    if (meshPtr == nullptr) {
                        throw std::runtime_error("nullptr mesh");
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

        void OGLRenderer::setTarget(const RenderTarget &t) {
            this->target = &t;
        }

        void OGLRenderer::setScene(const RenderScene &s) {
            this->scene = &s;
        }

        void OGLRenderer::setViewport(Vec2i offset, Vec2i size) {
            this->viewportOffset = offset;
            this->viewportSize = size;
        }

        void OGLRenderer::setClear(bool cColor, bool cDepth, bool cStencil) {
            this->clearColor = cColor;
            this->clearDepth = cDepth;
            this->clearStencil = cStencil;
        }

        void OGLRenderer::setClearColor(ColorRGBA cColor) {
            this->clearColorValue = cColor;
        }

        void OGLRenderer::setMultiSample(bool sample) {
            this->multiSample = sample;
        }

        void OGLRenderer::render() {
            if (target == nullptr)
                throw std::runtime_error("Null target");
            if (scene == nullptr)
                throw std::runtime_error("Null scene");

            glClearColor((float) clearColorValue.r() / (float) 255,
                         (float) clearColorValue.g() / (float) 255,
                         (float) clearColorValue.b() / (float) 255,
                         (float) clearColorValue.a() / (float) 255);

            if (multiSample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);

            auto &fb = dynamic_cast<const OGLFrameBuffer &>(*target);

            GLint vpData[4];
            glGetIntegerv(GL_VIEWPORT, vpData);

            glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);

            glBindFramebuffer(GL_FRAMEBUFFER, fb.getFBO());
            renderScene(*scene, clearColor, clearDepth, clearStencil);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(vpData[0], vpData[1], vpData[2], vpData[3]);

            checkGLError("OGLRenderer::render");
        }
    }
}