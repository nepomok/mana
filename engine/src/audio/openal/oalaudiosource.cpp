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

#include "audio/openal/oalaudiosource.hpp"
#include "audio/openal/oalcheckerror.hpp"
#include "audio/openal/oalaudiobuffer.hpp"

#include <AL/al.h>

namespace mana {
    int convertType(AudioSource::SourceType type) {
        switch (type) {
            case AudioSource::UNDETERMINED:
                return AL_UNDETERMINED;
            case AudioSource::STATIC:
                return AL_STATIC;
            case AudioSource::STREAMING:
                return AL_STREAMING;
        }
        throw std::runtime_error("Unrecognized type");
    }

    OALAudioSource::OALAudioSource(ALuint sourceHandle) : handle(sourceHandle) {}

    OALAudioSource::~OALAudioSource() {
        alDeleteSources(1, &handle);
        checkOALError();
    }

    void OALAudioSource::play() {
        alSourcePlay(handle);
        checkOALError();
    }

    void OALAudioSource::pause() {
        alSourcePause(handle);
        checkOALError();
    }

    void OALAudioSource::stop() {
        alSourceStop(handle);
        checkOALError();
    }

    void OALAudioSource::rewind() {
        alSourceRewind(handle);
        checkOALError();
    }

    void OALAudioSource::setPitch(float pitch) {
        alSourcef(handle, AL_PITCH, pitch);
        checkOALError();
    }

    void OALAudioSource::setGain(float gain) {
        alSourcef(handle, AL_GAIN, gain);
        checkOALError();
    }

    void OALAudioSource::setMaxDistance(float maxDistance) {
        alSourcef(handle, AL_MAX_DISTANCE, maxDistance);
        checkOALError();
    }

    void OALAudioSource::setRolloffFactor(float rollOffFactor) {
        alSourcef(handle, AL_ROLLOFF_FACTOR, rollOffFactor);
        checkOALError();
    }

    void OALAudioSource::setReferenceDistance(float referenceDistance) {
        alSourcef(handle, AL_REFERENCE_DISTANCE, referenceDistance);
        checkOALError();
    }

    void OALAudioSource::setMininumGain(float minGain) {
        alSourcef(handle, AL_MIN_GAIN, minGain);
        checkOALError();
    }

    void OALAudioSource::setMaximumGain(float maxGain) {
        alSourcef(handle, AL_MAX_GAIN, maxGain);
        checkOALError();
    }

    void OALAudioSource::setConeOuterGain(float coneOuterGain) {
        alSourcef(handle, AL_CONE_OUTER_GAIN, coneOuterGain);
        checkOALError();
    }

    void OALAudioSource::setConeInnerAngle(float innerAngle) {
        alSourcef(handle, AL_CONE_INNER_ANGLE, innerAngle);
        checkOALError();
    }

    void OALAudioSource::setConeOuterAngle(float outerAngle) {
        alSourcef(handle, AL_CONE_OUTER_ANGLE, outerAngle);
        checkOALError();
    }

    void OALAudioSource::setPosition(Vec3f position) {
        alSource3f(handle, AL_POSITION, position.x, position.y, position.z);
        checkOALError();
    }

    void OALAudioSource::setVelocity(Vec3f velocity) {
        alSource3f(handle, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        checkOALError();
    }

    void OALAudioSource::setDirection(Vec3f direction) {
        alSource3f(handle, AL_DIRECTION, direction.x, direction.y, direction.z);
        checkOALError();
    }

    void OALAudioSource::setSourceRelative(bool relative) {
        alSourcei(handle, AL_SOURCE_RELATIVE, relative);
        checkOALError();
    }

    void OALAudioSource::setSourceType(AudioSource::SourceType type) {
        alSourcei(handle, AL_SOURCE_TYPE, convertType(type));
        checkOALError();
    }

    void OALAudioSource::setLooping(bool looping) {
        alSourcei(handle, AL_LOOPING, looping);
        checkOALError();
    }

    void OALAudioSource::setBuffer(const AudioBuffer &buffer) {
        auto &b = dynamic_cast<const OALAudioBuffer &>(buffer);
        alSourcei(handle, AL_BUFFER, b.handle);
        checkOALError();
    }

    void OALAudioSource::queueBuffers(std::vector<const AudioBuffer *> buffers) {
        ALuint b[buffers.size()];
        for (int i = 0; i < buffers.size(); i++) {
            auto *ob = dynamic_cast<const OALAudioBuffer *>(buffers[i]);
            b[i] = ob->handle;
            bufferMapping[ob->handle] = ob;
        }
        alSourceQueueBuffers(handle, buffers.size(), b);
        checkOALError();
    }

    std::vector<const AudioBuffer *> OALAudioSource::unqueueBuffers() {
        int available;
        alGetSourcei(handle, AL_BUFFERS_PROCESSED, &available);
        checkOALError();
        ALuint b[available];
        alSourceUnqueueBuffers(handle, available, b);
        checkOALError();
        std::vector<const AudioBuffer *> ret;
        for (int i = 0; i < available; i++) {
            ALuint v = b[i];
            ret.emplace_back(bufferMapping.at(v));
            bufferMapping.erase(v);
        }
        return ret;
    }

    ALuint OALAudioSource::getHandle() {
        return handle;
    }
}