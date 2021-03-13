#ifndef MANA_WINDOWLISTENER_HPP
#define MANA_WINDOWLISTENER_HPP

#include "engine/math/vector2.hpp"

namespace mana {
    class WindowListener {
    public:
        virtual void onWindowClose() {};

        virtual void onWindowMove(Vec2i pos) {};

        virtual void onWindowResize(Vec2i size) {};

        virtual void onWindowRefresh() {};

        virtual void onWindowFocus(bool focused) {};

        virtual void onWindowMinimize() {};

        virtual void onWindowMaximize() {};

        virtual void onWindowContentScale(Vec2f scale) {}

        virtual void onFramebufferResize(Vec2i size) {}
    };
}

#endif //MANA_WINDOWLISTENER_HPP
