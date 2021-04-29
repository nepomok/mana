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

#include "engine/render/renderer3d.hpp"
#include "engine/math/rotation.hpp"

#include "render/opengl/oglglslinject.hpp"

namespace mana {
    std::string Renderer3D::preprocessGlsl(std::string shader) {
        auto index = shader.find(SHADER_INCLUDE);
        if (index != std::string::npos) {
            std::string start = shader.substr(0, index);
            std::string end = shader.substr(index + SHADER_INCLUDE.length());
            return start + SHADER_INJECT + end;
        }
        return shader;
    }

    Renderer3D::Renderer3D() = default;

    Renderer3D::Renderer3D(Renderer &r, RenderAllocator &a) : ren(&r), alloc(&a) {}
    
    void Renderer3D::setCamera(const Camera &cam) {
        camera = &cam;
    }

    void Renderer3D::setDirectionalLights(const std::vector<DirectionalLight> &lights) {
        dir = lights;
    }

    void Renderer3D::setPointLights(const std::vector<PointLight> &lights) {
        point = lights;
    }

    void Renderer3D::setSpotLights(const std::vector<SpotLight> &lights) {
        spot = lights;
    }

    void Renderer3D::setEnableShadowMapping(bool shadowMapping) {

    }

    void Renderer3D::draw(const RenderTarget &target, const std::vector<RenderCommand> &commands) {
        ren->renderBegin(target);
        for (const auto &command : commands) {
            if (camera == nullptr)
                throw std::runtime_error("Null camera");
            if (ren == nullptr)
                throw std::runtime_error("Null renderer");

            Mat4f model, view, projection;
            model = MatrixMath::translate(command.transform.position);
            model = model * MatrixMath::scale(command.transform.scale);
            model = model * MatrixMath::rotate(command.transform.rotation);

            view = camera->view();
            projection = camera->projection();

            ShaderProgram &shader = *command.shader;

            shader.setMat4("MANA_M", model);
            shader.setMat4("MANA_V", view);
            shader.setMat4("MANA_P", projection);
            shader.setMat4("MANA_MVP", projection * view * model);

            int i = 0;
            for (auto &light : point) {
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
            for (auto &light : spot) {
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
            for (auto &light : dir) {
                std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                shader.setVec3(name + "direction", light.direction);
                shader.setVec3(name + "ambient", light.ambient);
                shader.setVec3(name + "diffuse", light.diffuse);
                shader.setVec3(name + "specular", light.specular);
            }

            shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", dir.size());
            shader.setInt("MANA_LIGHT_COUNT_POINT", point.size());
            shader.setInt("MANA_LIGHT_COUNT_SPOT", spot.size());

            shader.setVec3("MANA_VIEWPOS", camera->transform.position);

            ren->addCommand(command);
        }
        ren->renderFinish();
    }
}