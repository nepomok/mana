#ifndef MANA_INPUT_HPP
#define MANA_INPUT_HPP

#include "engine/input/key.hpp"
#include "engine/input/mouse.hpp"
#include "engine/input/inputlistener.hpp"

namespace mana {
    class Input {
    public:
        virtual ~Input() = default;

        virtual bool getKeyDown(Key key) = 0;

        virtual Mouse getMouse() = 0;

        virtual void registerListener(InputListener &listener) = 0;

        virtual void unregisterListener(InputListener &listener) = 0;

        virtual void setClipboardText(std::string text) = 0;

        virtual std::string getClipboardText() = 0;
    };
}

#endif //MANA_INPUT_HPP
