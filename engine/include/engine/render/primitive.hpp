#ifndef MANA_PRIMITIVE_HPP
#define MANA_PRIMITIVE_HPP

namespace mana {
    /**
     * The geometric primitives supported by the graphics api abstraction.
     * Note that individual faces in the meshes have to consist of the identical primitive (Triangulate, etc.).
     * Triangulation is the responsibility of the mesh loader.
     */
    enum Primitive {
        POINT,
        LINE,
        TRI,
        QUAD
    };
}

#endif //MANA_PRIMITIVE_HPP
