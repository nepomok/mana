#ifndef MANA_TRANSFORM_HPP
#define MANA_TRANSFORM_HPP

#include "engine/math/vector3.hpp"

namespace mana {
    struct Transform {
        Vec3f position;
        Vec3f rotation;
        Vec3f scale = Vec3f(1);

        Transform() = default;

        Transform(Vec3f position, Vec3f rotation, Vec3f scale) : position(position),
                                                                 rotation(rotation),
                                                                 scale(scale) {}
    };
}

#endif //MANA_TRANSFORM_HPP
