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

#include "engine/asset/vertex.hpp"

#include "engine/asset/asset.hpp"

namespace engine {
    struct Mesh : public Asset {
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

        ~Mesh() override = default;

        Asset *clone() override {
            return new Mesh(*this);
        }
    };
}

#endif //MANA_MESH_HPP
