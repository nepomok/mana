#ifndef MANA_RIGIDBODYDATA_HPP
#define MANA_RIGIDBODYDATA_HPP

#include "engine/ecs/component.hpp"

namespace mana {

    struct RigidBody : public Component {
        bool isStatic;
        bool isKinematic;
    };
}

#endif //MANA_RIGIDBODYDATA_HPP
