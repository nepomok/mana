#ifndef MANA_TRANSFORMANIMATIONSYSTEM_HPP
#define MANA_TRANSFORMANIMATIONSYSTEM_HPP


#include "ecs/system.hpp"

#include "components/transformanimationcomponent.hpp"

using namespace engine;

class TransformAnimationSystem : public System {
public:
    void update(float deltaTime, EntityManager &entityManager) override {
        auto &componentManager = entityManager.getComponentManager();
        for (auto &pair: componentManager.getPool<TransformAnimationComponent>()) {
            auto transform = componentManager.lookup<TransformComponent>(pair.first);

            transform.transform.setPosition(transform.transform.getPosition() + pair.second.translation * deltaTime);
            transform.transform.setRotation(transform.transform.getRotation() * Quaternion(pair.second.rotation * deltaTime));

            componentManager.update<TransformComponent>(pair.first, transform);
        }
    }
};

#endif //MANA_TRANSFORMANIMATIONSYSTEM_HPP
