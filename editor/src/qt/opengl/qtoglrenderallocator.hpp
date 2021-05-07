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

#ifndef MANA_QtOGLRENDERALLOCATOR_HPP
#define MANA_QtOGLRENDERALLOCATOR_HPP

#include "engine/render/renderallocator.hpp"

#include <QOpenGLFunctions_3_3_Core>

namespace mana {
    namespace opengl {
        class QtOGLRenderAllocator : public RenderAllocator, public QOpenGLFunctions_3_3_Core {
        public:
            RenderTarget *allocateRenderTarget(int width, int height) override;

            TextureBuffer *allocateTextureBuffer(TextureBuffer::Attributes attributes) override;

            MeshBuffer *allocateMeshBuffer(const Mesh &mesh) override;

            MeshBuffer *allocateInstancedMeshBuffer(const Mesh &mesh,
                                                    const std::vector<Transform> &offsets) override;

            ShaderProgram *allocateShaderProgram(const std::string &vertexShader, const std::string &fragmentShader,
                                                 const std::map<std::string, std::string> &macros,
                                                 const std::function<std::string(
                                                         const char *)> &includeCallback) override;
        };
    }
}

#endif //MANA_QtOGLRENDERALLOCATOR_HPP
