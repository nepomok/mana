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

#ifndef MANA_MATERIAL_HPP
#define MANA_MATERIAL_HPP

#include "engine/render/imagebuffer.hpp"
#include "engine/render/shaderprogram.hpp"

#include "engine/resource/resource.hpp"

namespace mana {
    struct Material {
        ShaderProgram *shader = nullptr;

        std::string name;

        ColorRGBA diffuseColor;
        ColorRGBA specularColor;
        ColorRGBA ambientColor;

        ImageBuffer<ColorRGBA> *diffuseTex = nullptr;
        ImageBuffer<ColorRGBA> *specularTex = nullptr;
        ImageBuffer<ColorRGBA> *ambientTex = nullptr;
        ImageBuffer<ColorRGBA> *emissiveTex = nullptr;
        ImageBuffer<ColorRGBA> *normalTex = nullptr;
        ImageBuffer<ColorRGBA> *roughnessTex = nullptr;

        bool wireframe;
        bool twosided;

        float opacity;
        float roughness;
    };
}

#endif //MANA_MATERIAL_HPP
