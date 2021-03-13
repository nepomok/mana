#ifndef MANA_RUNTIME_HPP
#define MANA_RUNTIME_HPP

#include "engine/ecs/node.hpp"

#include "engine/display/displayapi.hpp"

namespace mana {
    namespace runtime {
        class Runtime {
        public:
            Runtime(DisplayAPI &displayApi);

            int run(const Node &rootNode);

        private:
            DisplayAPI *displayApi;
        };
    }
}

#endif //MANA_RUNTIME_HPP
