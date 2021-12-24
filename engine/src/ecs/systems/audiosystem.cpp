#include "engine/ecs/systems/audiosystem.hpp"

#include "engine/ecs/components/audio/audiolistenercomponent.hpp"
#include "engine/ecs/components/audio/audiosourcecomponent.hpp"

#define AUDIO_POS_SCALE 1

namespace engine {
    AudioSystem::AudioSystem(AudioDevice &device, AssetManager &assetManager)
            : device(device), assetManager(assetManager) {
        context = device.createContext();
        context->makeCurrent();
    }

    void AudioSystem::start(EntityManager &entityManager) {
        entityManager.getComponentManager().getPool<AudioSourceComponent>().addListener(this);
    }

    void AudioSystem::stop(EntityManager &entityManager) {
        entityManager.getComponentManager().getPool<AudioSourceComponent>().removeListener(this);
    }

    void AudioSystem::update(float deltaTime, EntityManager &entityManager) {
        auto &componentManager = entityManager.getComponentManager();

        for (auto &pair: componentManager.getPool<AudioListenerComponent>()) {
            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            auto &listener = context->getListener();
            listener.setPosition(transform.transform.getPosition() * AUDIO_POS_SCALE);
            listener.setOrientation({transform.transform.getPosition()},
                                    transform.transform.getRotation().getEulerAngles());
            listener.setVelocity(pair.second.velocity);
        }

        for (auto &pair: componentManager.getPool<AudioSourceComponent>()) {
            auto &comp = pair.second;
            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            auto &source = sources.at(pair.first);

            source->setPosition(transform.transform.getPosition() * AUDIO_POS_SCALE);
            source->setLooping(comp.loop);
            source->setVelocity(comp.velocity);

            //TODO: Source Volume and Distance

            if (comp.play && !comp.playing) {
                source->play();
                comp.playing = true;
            } else if (!comp.play && comp.playing) {
                source->pause();
                comp.playing = false;
            }
        }
    }

    void AudioSystem::onComponentCreate(const Entity &entity, const AudioSourceComponent &component) {
        if (!component.audioPath.empty()) {
            auto handle = AssetHandle<Audio>(component.audioPath, assetManager);

            buffers[entity] = context->createBuffer();
            buffers[entity]->upload(handle.get().buffer, handle.get().format, handle.get().frequency);

            sources[entity] = context->createSource();
            sources[entity]->setBuffer(*buffers[entity]);
        }
    }

    void AudioSystem::onComponentDestroy(const Entity &entity, const AudioSourceComponent &component) {
        sources.erase(entity);
        buffers.erase(entity);
    }

    void AudioSystem::onComponentUpdate(const Entity &entity,
                                        const AudioSourceComponent &oldValue,
                                        const AudioSourceComponent &newValue) {
        if (!(oldValue.audioPath == newValue.audioPath)) {
            sources[entity]->stop();
            auto handle = AssetHandle<Audio>(newValue.audioPath, assetManager);
            buffers.at(entity)->upload(handle.get().buffer, handle.get().format, handle.get().frequency);
            sources.at(entity)->setBuffer(*buffers.at(entity));
        }
    }
}