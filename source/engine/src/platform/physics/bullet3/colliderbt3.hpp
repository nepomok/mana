#ifndef MANA_COLLIDERBT3_HPP
#define MANA_COLLIDERBT3_HPP

#include "platform/physics/3d/collider3d.hpp"

namespace engine {
    class ColliderBt3 : Collider3D {
    public:
        void setShape(const std::vector<Vec3f> &vertices) override;

        void setShape(const std::vector<Vec3f> &vertices, const std::vector<std::size_t> &indices) override;

        void setDensity(float density) override;

        float getDensity() override;
    };
}

#endif //MANA_COLLIDERBT3_HPP
