#ifndef MANA_AUDIOSYSTEM_HPP
#define MANA_AUDIOSYSTEM_HPP


#include "ecs/system.hpp"


#include "ecs/components/audio/audiosourcecomponent.hpp"
#include "ecs/components/transformcomponent.hpp"

#include "platform/audio/audiodevice.hpp"

namespace engine {
    class AudioSystem : public System, ComponentPool<AudioSourceComponent>::Listener {
    public:
        explicit AudioSystem(AudioDevice &device, AssetManager &assetManager);

        ~AudioSystem() override = default;

        void start(EntityManager &entityManager) override;

        void stop(EntityManager &entityManager) override;

        void update(float deltaTime, EntityManager &entityManager) override;

    private:
        void onComponentCreate(const Entity &entity, const AudioSourceComponent &component) override;

        void onComponentDestroy(const Entity &entity, const AudioSourceComponent &component) override;

        void onComponentUpdate(const Entity &entity, const AudioSourceComponent &oldValue,
                               const AudioSourceComponent &newValue) override;

    private:
        AudioDevice &device;
        AssetManager &assetManager;

        std::unique_ptr<AudioContext> context;

        std::map<Entity, std::unique_ptr<AudioSource>> sources;
        std::map<Entity, std::unique_ptr<AudioBuffer>> buffers;
    };
}

#endif //MANA_AUDIOSYSTEM_HPP
