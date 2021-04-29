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

        void OGLRenderer::setCamera(const Camera &camera) {
            view = camera.view();
            projection = camera.projection();
            cameraTransform = camera.transform;
        }

        void OGLRenderer::setViewport(Vec2i offset, Vec2i size) {
            this->viewportOffset = offset;
            this->viewportSize = size;
        }

        void OGLRenderer::setClear(bool cC, bool cD, bool cS) {
            this->clearColor = cC;
            this->clearDepth = cD;
            this->clearStencil = cS;
        }

        void OGLRenderer::setClearColor(ColorRGBA c) {
            this->clearColorValue = c;
        }

        void OGLRenderer::setMultiSample(bool s) {
            this->multiSample = s;
        }

        void OGLRenderer::setDirectionalLights(const std::vector<DirectionalLight> &lights) {
            this->directionalLights = lights;
        }

        void OGLRenderer::setPointLights(const std::vector<PointLight> &lights) {
            this->pointLights = lights;
        }

        void OGLRenderer::setSpotLights(const std::vector<SpotLight> &lights) {
            this->spotLights = lights;
        }

        void OGLRenderer::renderBegin(const RenderTarget &target) {
            glClearColor((float) clearColorValue.r() / (float) 255,
                         (float) clearColorValue.g() / (float) 255,
                         (float) clearColorValue.b() / (float) 255,
                         (float) clearColorValue.a() / (float) 255);

            if (multiSample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);

            auto &fb = dynamic_cast<const OGLFrameBuffer &>(target);

            GLint vpData[4];
            glGetIntegerv(GL_VIEWPORT, vpData);

            glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);

            glBindFramebuffer(GL_FRAMEBUFFER, fb.getFBO());

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
        }

        void OGLRenderer::addCommand(const RenderCommand &command) {
            Mat4f model = MatrixMath::translate(command.transform.position);
            model = model * MatrixMath::scale(command.transform.scale);
            model = model * MatrixMath::rotate(command.transform.rotation);

            //Bind textures
            for (int i = 0; i < command.textureObjects.size(); i++) {
                auto *textureObject = command.textureObjects.at(i);
                if (textureObject == nullptr) {
                    throw std::runtime_error("nullptr texture");
                }
                auto &texture = dynamic_cast<const OGLRenderTexture &>(*textureObject);
                glActiveTexture(getTextureSlot(i));
                glBindTexture(OGLTypeConverter::convert(texture.attributes.textureType), texture.handle);
            }

            //Bind shader program
            ShaderProgram *sp = command.shader;
            if (sp == nullptr) {
                throw std::runtime_error("nullptr shaderprogram");
            }
            auto &shader = dynamic_cast<OGLShaderProgram &>(*sp);
            shader.activate();

            //Assign shader uniforms
            //Matrices
            shader.setMat4("MANA_M", model);
            shader.setMat4("MANA_V", view);
            shader.setMat4("MANA_P", projection);
            shader.setMat4("MANA_MVP", projection * view * model);

            //Lights
            int i = 0;
            for (auto &light : pointLights) {
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
            for (auto &light : spotLights) {
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
            for (auto &light : directionalLights) {
                std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                shader.setVec3(name + "direction", light.direction);
                shader.setVec3(name + "ambient", light.ambient);
                shader.setVec3(name + "diffuse", light.diffuse);
                shader.setVec3(name + "specular", light.specular);
            }

            shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", directionalLights.size());
            shader.setInt("MANA_LIGHT_COUNT_POINT", pointLights.size());
            shader.setInt("MANA_LIGHT_COUNT_SPOT", spotLights.size());

            shader.setVec3("MANA_VIEWPOS", cameraTransform.position);

            //Setup per model depth, stencil, culling and blend states
            if (command.enableDepthTest) {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(OGLTypeConverter::convert(command.depthTestMode));
                if (command.depthTestWrite)
                    glDepthMask(GL_TRUE);
                else
                    glDepthMask(GL_FALSE);
            } else {
                glDisable(GL_DEPTH_TEST);
            }

            if (command.enableStencilTest) {
                glEnable(GL_STENCIL_TEST);
                glStencilMask(OGLTypeConverter::convertPrimitive(command.stencilTestMask));
                glStencilFunc(OGLTypeConverter::convert(command.stencilMode),
                              OGLTypeConverter::convertPrimitive(command.stencilReference),
                              OGLTypeConverter::convertPrimitive(command.stencilFunctionMask));
                glStencilOp(OGLTypeConverter::convert(command.stencilFail),
                            OGLTypeConverter::convert(command.stencilDepthFail),
                            OGLTypeConverter::convert(command.stencilPass));
            } else {
                glDisable(GL_STENCIL_TEST);
            }

            if (command.enableFaceCulling) {
                glEnable(GL_CULL_FACE);
                glCullFace(OGLTypeConverter::convert(command.faceCullMode));
                if (command.faceCullClockwiseWinding)
                    glFrontFace(GL_CW);
                else
                    glFrontFace(GL_CCW);
            } else {
                glDisable(GL_CULL_FACE);
            }

            if (command.enableBlending) {
                glEnable(GL_BLEND);
                glBlendFunc(OGLTypeConverter::convert(command.blendSourceMode),
                            OGLTypeConverter::convert(command.blendDestinationMode));
            } else {
                glDisable(GL_BLEND);
            }

            //Bind VAOs and draw.
            for (auto meshPtr : command.meshObjects) {
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

            checkGLError("OGLRenderer::addCommand");
        }

        void OGLRenderer::addCommands(const std::vector<RenderCommand> &commands) {
            for (const auto &command : commands) {
                addCommand(command);
            }
        }

        void OGLRenderer::renderFinish() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            checkGLError("OGLRenderer::renderFinish");
        }
    }
}