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

#ifndef MANA_MESH_HPP
#define MANA_MESH_HPP

#include <vector>

#include "engine/math/vector3.hpp"
#include "engine/math/vector2.hpp"

namespace mana {
    struct Vertex {
        float data[8];

        Vertex(Vec3f position, Vec3f normal, Vec2f uv) : data() {
            data[0] = position.x;
            data[1] = position.y;
            data[2] = position.z;
            data[3] = normal.x;
            data[4] = normal.y;
            data[5] = normal.z;
            data[6] = uv.x;
            data[7] = uv.y;
        }

        Vertex(Vec3f position, Vec2f uv) : Vertex(position, {}, uv) {}

        explicit Vertex(Vec3f position) : Vertex(position, {}, {}) {}

        Vertex() = default;
    };

    struct Mesh {
        enum Primitive {
            POINT,
            LINE,
            TRI,
            QUAD
        };

        bool indexed = false;
        Primitive primitive = POINT;
        std::vector<Vertex> vertices;
        std::vector<uint> indices;

        Mesh() = default;

        Mesh(bool indexed, Primitive primitive, std::vector<Vertex> vertices,
             std::vector<uint> indices) :
                indexed(indexed), primitive(primitive), vertices(std::move(vertices)),
                indices(std::move(indices)) {}

        Mesh(Primitive primitive, std::vector<Vertex> vertices, std::vector<uint> indices) :
                indexed(true), primitive(primitive), vertices(std::move(vertices)), indices(std::move(indices)) {}

        Mesh(Primitive primitive, std::vector<Vertex> vertices) :
                indexed(false), primitive(primitive), vertices(std::move(vertices)), indices() {}
    };
}

#endif //MANA_MESH_HPP
