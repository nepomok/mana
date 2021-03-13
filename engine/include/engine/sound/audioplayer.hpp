#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include <vector>

#include "engine/sound/audiobuffer.hpp"

namespace mana {
    class AudioPlayer {
    public:
        AudioPlayer(int numberOfSlots);

        ~AudioPlayer();

        void load(int slot,
                  AudioBuffer &buffer,
                  long offset = 0,
                  float volume = 1.0f,
                  bool playing = true,
                  bool looping = true);

        void unload(int slot);

        void unload();

        void play(int slot);

        void pause(int slot);

        void play();

        void pause();

        bool getLooping(int slot);

        void setLooping(int slot, bool looping);

        bool getPlaying(int slot);

        void setPlaying(int slot, bool playing);

        float getVolume(int slot);

        void setVolume(int slot, float volume);

        void setVolume(float volume);

        float getProgress(int slot);

        void setProgress(int slot, float progress);
    };
}

#endif // AUDIOPLAYER_HPP