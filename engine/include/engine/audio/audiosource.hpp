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

#ifndef MANA_AUDIOSOURCE_HPP
#define MANA_AUDIOSOURCE_HPP

#include "engine/audio/audiobuffer.hpp"

#include "engine/math/vector3.hpp"

namespace mana {
    class AudioSource {
    public:
        enum SourceType {
            UNDETERMINED,
            STATIC,
            STREAMING
        };

        enum SourceState {
            STOPPED,
            PLAYING
        };

        virtual ~AudioSource() = default;

        virtual void play() = 0;

        virtual void pause() = 0;

        virtual void stop() = 0;

        virtual void rewind() = 0;

        virtual void setPitch(float pitch) = 0;

        virtual void setGain(float gain) = 0;

        virtual void setMaxDistance(float maxDistance) = 0;

        virtual void setRolloffFactor(float rollOffFactor) = 0;

        virtual void setReferenceDistance(float referenceDistance) = 0;

        virtual void setMininumGain(float minGain) = 0;

        virtual void setMaximumGain(float maxGain) = 0;

        virtual void setConeOuterGain(float coneOuterGain) = 0;

        virtual void setConeInnerAngle(float innerAngle) = 0;

        virtual void setConeOuterAngle(float outerAngle) = 0;

        virtual void setPosition(Vec3f position) = 0;

        virtual void setVelocity(Vec3f velocity) = 0;

        virtual void setDirection(Vec3f direction) = 0;

        virtual void setSourceRelative(bool relative) = 0;

        virtual void setSourceType(SourceType type) = 0;

        virtual void setLooping(bool looping) = 0;

        virtual void setBuffer(const AudioBuffer &buffer) = 0;

        virtual void queueBuffers(std::vector<const AudioBuffer *> buffers) = 0;

        virtual std::vector<const AudioBuffer *> unqueueBuffers() = 0;
    };
}

#endif //MANA_AUDIOSOURCE_HPP
