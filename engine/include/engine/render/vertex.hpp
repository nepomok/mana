#ifndef MANA_VERTEX_HPP
#define MANA_VERTEX_HPP

#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"

namespace mana {
    struct Vertex {
        float data[8]; // Public member ensures that address of vertex instance = position[0], array to ensure contiguous memory.

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
}

#endif //MANA_VERTEX_HPP
