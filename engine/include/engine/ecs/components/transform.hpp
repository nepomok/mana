#ifndef MANA_TRANSFORMDATA_HPP
#define MANA_TRANSFORMDATA_HPP

#include "engine/ecs/component.hpp"

#include "engine/math/vector3.hpp"

namespace mana {
    struct Transform : public Component {
        //Todo: Transform parenting

        Vec3f position;
        Vec3f rotation;
        Vec3f scale;
    };
}
#endif //MANA_TRANSFORMDATA_HPP
