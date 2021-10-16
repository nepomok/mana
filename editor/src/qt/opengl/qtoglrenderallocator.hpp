/**
 *  Mana - 3D Game Engine
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

#ifndef MANA_QTOGLRENDERALLOCATOR_HPP
#define MANA_OGLRENDERALLOCATOR_HPP

#include "engine.hpp"

#include <QOpenGLFunctions_3_3_Core>

namespace engine {
    namespace opengl {
        class QtOGLRenderAllocator : public RenderAllocator, public QOpenGLFunctions_3_3_Core {
        public:
            RenderTarget *createRenderTarget(Vec2i size, int samples) override;

            TextureBuffer *createTextureBuffer(TextureBuffer::Attributes attributes) override;

            MeshBuffer *createMeshBuffer(const Mesh &mesh) override;

            MeshBuffer *createInstancedMeshBuffer(const Mesh &mesh, const std::vector<Transform> &offsets) override;

            ShaderProgram *createShaderProgram(const ShaderSource &vertexShader,
                                               const ShaderSource &fragmentShader) override;

            ShaderProgram *createShaderProgram(const ShaderSource &vertexShader,
                                               const ShaderSource &geometryShader,
                                               const ShaderSource &fragmentShader) override;
        };
    }
}
#endif //MANA_QTOGLRENDERALLOCATOR_HPP
