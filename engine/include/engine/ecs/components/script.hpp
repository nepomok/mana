#ifndef MANA_SCRIPTDATA_HPP
#define MANA_SCRIPTDATA_HPP

#include "engine/ecs/component.hpp"
#include "engine/io/resource.hpp"

namespace mana {

    struct Script : public Component {
        enum Type {
            PYTHON,
            MONO
        } type;

        Resource <std::string> *source;
    };
}

#endif //MANA_SCRIPTDATA_HPP
