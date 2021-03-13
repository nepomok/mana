#ifndef MANA_MOUSE_HPP
#define MANA_MOUSE_HPP

#include "engine/math/vector2.hpp"

namespace mana {
    struct Mouse {
        Vec2d position;
        bool leftButtonDown;
        bool middleButtonDown;
        bool rightButtonDown;

        Mouse(Vec2d position, bool leftBtn, bool midBtn, bool rightBtn)
                : position(position), leftButtonDown(leftBtn), middleButtonDown(midBtn), rightButtonDown(rightBtn) {
        }

        Mouse()
                : position(0), leftButtonDown(false), middleButtonDown(false), rightButtonDown(false) {
        }
    };
}

#endif //MANA_MOUSE_HPP
