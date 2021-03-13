#ifndef MANA_RENDERTRANSFORM_HPP
#define MANA_RENDERTRANSFORM_HPP

#include "engine/math/vector3.hpp"

namespace mana {
    struct RenderTransform {
        Vec3f position;
        Vec3f rotation;
        Vec3f scale = Vec3f(1);

        RenderTransform() = default;

        RenderTransform(Vec3f position, Vec3f rotation, Vec3f scale) : position(position),
                                                                       rotation(rotation),
                                                                       scale(scale) {}
    };
}

#endif //MANA_RENDERTRANSFORM_HPP
