/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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