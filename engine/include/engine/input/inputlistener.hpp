#ifndef MANA_INPUTLISTENER_HPP
#define MANA_INPUTLISTENER_HPP

#include <string>

#include "engine/input/key.hpp"
#include "engine/input/mousekey.hpp"

namespace mana {
    class InputListener {
    public:
        virtual void onKeyDown(Key key) {};

        virtual void onKeyUp(Key key) {};

        virtual void onMouseMove(double xpos, double ypos) {};

        virtual void onMouseWheelScroll(double ammount) {};

        virtual void onMouseKeyDown(MouseKey key) {};

        virtual void onMouseKeyUp(MouseKey key) {};

        virtual void onTextInput(std::string text) {};

        //Todo: Gamepad support
    };
}

#endif //MANA_INPUTLISTENER_HPP
