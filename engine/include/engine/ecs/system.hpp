#ifndef MANA_SYSTEM_HPP
#define MANA_SYSTEM_HPP

#include "engine/ecs/scene.hpp"

namespace mana {
    /**
     * Systems provide logic.
     *
     * Only systems and scripts invoked by a script system invoke logic on the renderCommand data.
     *
     * Examples are invoking script lifecycle, handle render, manage memory allocation etc.
     */
    class System {
    public:
        virtual ~System() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void update(float deltaTime, const Scene &rootNode) = 0;

        virtual void onNodeCreated(const Node &node) {};

        virtual void onComponentCreated(const Node &node, const Component &component) {};

        virtual void onNodeRemoved(const Node &node) {};

        virtual void onComponentRemoved(const Node &node, const Component &component) {};
    };
}
#endif //MANA_SYSTEM_HPP
