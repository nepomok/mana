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

#ifndef MANA_RENDERUNIT_HPP
#define MANA_RENDERUNIT_HPP

#include <utility>

#include "engine/math/transform.hpp"
#include "engine/render/mesh.hpp"
#include "engine/render/3d/material.hpp"

namespace mana {
    struct RenderUnit {
        RenderUnit() : transform(), material(), meshes() {}

        RenderUnit(Transform t, Material *material, std::vector<Mesh *> meshes) : transform(t),
                                                                                  material(material),
                                                                                  meshes(std::move(meshes)) {}

        Transform transform;

        Material *material;
        std::vector<Mesh *> meshes;

        bool outline = false;
        ColorRGB outlineColor;
        float outlineScale = 1.1f;
    };
}

#endif //MANA_RENDERUNIT_HPP
