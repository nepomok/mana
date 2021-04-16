#ifndef MANA_VERTEX_HPP
#define MANA_VERTEX_HPP

#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"

namespace mana {
    struct Vertex {
        Vec3f position;
        Vec3f normal;
        Vec2f uv;

        Vertex() : position(), normal(), uv() {}

        Vertex(Vec3f position, Vec3f normal, Vec2f uv) : position(position), normal(normal), uv(uv) {}

        Vertex(Vec3f position, Vec2f uv) : position(position), normal(), uv(uv) {}

        explicit Vertex(Vec3f position) : position(position), uv() {}
    };
}

#endif //MANA_VERTEX_HPP
