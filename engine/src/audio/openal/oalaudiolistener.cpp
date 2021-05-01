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

#include "audio/openal/oalaudiolistener.hpp"
#include "audio/openal/oalcheckerror.hpp"

#include <AL/al.h>

namespace mana {
    OALAudioListener::OALAudioListener(AudioContext *context) : context(context) {
    }

    OALAudioListener::~OALAudioListener() = default;

    void OALAudioListener::setGain(float gain) {
        context->makeCurrent();
        alListenerf(AL_GAIN, gain);
        checkOALError();
    }

    void OALAudioListener::setPosition(Vec3f position) {
        context->makeCurrent();
        alListener3f(AL_POSITION, position.x, position.y, position.z);
        checkOALError();
    }

    void OALAudioListener::setVelocity(Vec3f velocity) {
        context->makeCurrent();
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        checkOALError();
    }

    void OALAudioListener::setOrientation(Vec3f at, Vec3f up) {
        context->makeCurrent();
        float v[6] = {at.x, at.y, at.z, up.x, up.y, up.z};
        alListenerfv(AL_ORIENTATION, v);
        checkOALError();
    }
}