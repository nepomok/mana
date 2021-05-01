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

#include <AL/al.h>
#include <AL/alext.h>

#include <vector>

#include "audio/openal/oalaudiobuffer.hpp"
#include "audio/openal/oalcheckerror.hpp"

namespace mana {
    int convertFormat(AudioFormat format) {
        switch (format) {
            case MONO8:
                return AL_FORMAT_MONO8;
            case MONO16:
                return AL_FORMAT_MONO16;
            case STEREO8:
                return AL_FORMAT_STEREO8;
            case STEREO16:
                return AL_FORMAT_STEREO16;
            case BFORMAT2D_16:
                return AL_FORMAT_BFORMAT2D_16;
            case BFORMAT3D_16:
                return AL_FORMAT_BFORMAT3D_16;
        }
        throw std::runtime_error("Unrecognized format");
    }

    OALAudioBuffer::OALAudioBuffer(ALuint handle) : handle(handle) {}

    OALAudioBuffer::~OALAudioBuffer() {
        alDeleteBuffers(1, &handle);
        checkOALError();
    }

    void OALAudioBuffer::upload(const std::vector<uint8_t> &buffer, AudioFormat format, unsigned int frequency) {
        alBufferData(handle, convertFormat(format), buffer.data(), buffer.size(), frequency);
        checkOALError();
    }
}