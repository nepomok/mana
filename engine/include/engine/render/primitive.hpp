#ifndef MANA_PRIMITIVE_HPP
#define MANA_PRIMITIVE_HPP

namespace mana {
    /**
     * The geometric primitives supported by the graphics api abstraction.
     * Note that individual faces in a the meshes have to consist of the identical primitive (Triangulate, etc.).
     *
     * In the future the engine api will contain a triangulation algorithm for the mesh objects, so that users may convert
     * arbitrary mesh objects to triangulated render meshes.
     */
    enum Primitive {
        POINT,
        LINE,
        TRI,
        QUAD
    };
}

#endif //MANA_PRIMITIVE_HPP
