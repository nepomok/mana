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

#include "engine/audio/audiodevice.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <stdexcept>

#include "audio/openal/oalaudiocontext.hpp"
#include "audio/openal/oalcheckerror.hpp"

namespace mana {
    AudioDevice::AudioDevice() {
        device = alcOpenDevice(nullptr);
        if (!device) {
            throw std::runtime_error("Failed to open default device");
        }
        checkOALError();
    }

    AudioDevice::AudioDevice(const std::string &n) {
        device = alcOpenDevice(n.c_str());
        if (!device) {
            throw std::runtime_error("Failed to open device " + n);
        }
        checkOALError();
    }

    AudioDevice::~AudioDevice() {
        alcCloseDevice(device);
        checkOALError();
    }

    AudioContext *AudioDevice::createContext() {
        auto c = alcCreateContext(device, nullptr);
        checkOALError();
        return new OALAudioContext(c);
    }

    std::vector<std::string> AudioDevice::getAvailableDevices() {
        const char *dev = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
        checkOALError();
        std::vector<std::string> ret;
        std::string tmp;
        bool gotzero = false;
        for (int i = 0; true; i++) {
            char c = dev[i];
            if (c == 0) {
                if (gotzero)
                    break;
                gotzero = true;
                ret.emplace_back(tmp);
                tmp.clear();
            } else {
                gotzero = false;
                tmp += c;
            }
        }
        return ret;
    }
}

