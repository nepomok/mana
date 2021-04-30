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

#include "engine/render/geometry/primitive.hpp"
#include "engine/render/geometry/vertex.hpp"

namespace mana {
    /**
     * The graphics api independent low level mesh definition, describing a mesh consisting of identical primitives.
     *
     * Defines a mesh with vertices and optionally indices which are drawn as the specified primitive.
     *
     * When using a non indexed mesh the vertices are red starting from index 0 using the specified primitive.
     * (Eg. TRI = { vertices[0] vertices[1] vertices[2] } , { vertices[3]...).
     *
     * Optionally users may pass a indices vector which is red starting from index 0 using the specified primitive.
     * (Eg. TRI = { vertices[indices[0]] vertices[indices[1]] vertices[indices[2]] } , { vertices[indices[3]]...).
     */
    struct Mesh {
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
