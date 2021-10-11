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

#include "audio/openal/openal.hpp"

#include "engine/audio/audiosource.hpp"

#include <map>

namespace engine {
    class OALAudioSource : public AudioSource {
    public:
        explicit OALAudioSource(ALuint sourceHandle);

        ~OALAudioSource() override;

        void play() override;

        void pause() override;

        void stop() override;

        void rewind() override;

        void setPitch(float pitch) override;

        float getPitch() override;

        void setGain(float gain) override;

        float getGain() override;

        void setMaxDistance(float maxDistance) override;

        float getMaxDistance() override;

        void setRollOffFactor(float rollOffFactor) override;

        float getRollOffFactor() override;

        void setReferenceDistance(float referenceDistance) override;

        float getReferenceDistance() override;

        void setMininumGain(float minGain) override;

        float getMinimumGain() override;

        void setMaximumGain(float maxGain) override;

        float getMaximumGain() override;

        void setConeOuterGain(float coneOuterGain) override;

        float getConeOuterGain() override;

        void setConeInnerAngle(float innerAngle) override;

        float getConeInnerAngle() override;

        void setConeOuterAngle(float outerAngle) override;

        float getConeOuterAngle() override;

        void setPosition(Vec3f position) override;

        Vec3f getPosition() override;

        void setVelocity(Vec3f velocity) override;

        Vec3f getVelocity() override;

        void setDirection(Vec3f direction) override;

        Vec3f getDirection() override;

        void setSourceRelative(bool relative) override;

        bool getSourceRelative() override;

        void setSourceType(SourceType type) override;

        SourceType getSourceType() override;

        void setLooping(bool looping) override;

        bool getLooping() override;

        void setBuffer(const AudioBuffer &buffer) override;

        void queueBuffers(std::vector<const AudioBuffer *> buffers) override;

        std::vector<const AudioBuffer *> unqueueBuffers() override;

        ALuint getHandle();

    private:
        ALuint handle;

        std::map<ALuint, const AudioBuffer *> bufferMapping; //Mapping for returning unqueued buffers.
    };
}