#ifndef MANA_RENDERMESH_HPP
#define MANA_RENDERMESH_HPP

#include <utility>
#include <vector>

#include "engine/render/primitive.hpp"
#include "engine/render/vertex.hpp"

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
    struct RenderMesh {
        bool indexed = false;
        Primitive primitive = POINT;
        std::vector<Vertex> vertices;
        std::vector<uint> indices;

        RenderMesh() = default;

        RenderMesh(bool indexed, Primitive primitive, std::vector<Vertex> vertices,
                   std::vector<uint> indices) :
                indexed(indexed), primitive(primitive), vertices(std::move(vertices)),
                indices(std::move(indices)) {}

        RenderMesh(Primitive primitive, std::vector<Vertex> vertices, std::vector<uint> indices) :
                indexed(true), primitive(primitive), vertices(std::move(vertices)), indices(std::move(indices)) {}

        RenderMesh(Primitive primitive, std::vector<Vertex> vertices) :
                indexed(false), primitive(primitive), vertices(std::move(vertices)), indices() {}
    };
}

#endif //MANA_MESH_HPP
