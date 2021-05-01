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

#include "engine/io/audiofile.hpp"

#include <sndfile.h>

#include <stdexcept>
#include <limits>
#include <cstring>

namespace mana {
    AudioFile::AudioFile() = default;

    AudioFile::AudioFile(const std::string &filePath) {
        loadFile(filePath);
    }

    AudioFile::~AudioFile() = default;

    void AudioFile::loadFile(const std::string &filePath) {
        //Based on this sample: https://github.com/kcat/openal-soft/blob/master/examples/alplay.c
        SF_INFO sfinfo;
        SNDFILE *sndfile = sf_open(filePath.c_str(), SFM_READ, &sfinfo);
        if (!sndfile) {
            throw std::runtime_error("Failed to open audio file at " + filePath);
        }
        if (sfinfo.frames<1
                          || sfinfo.frames>(sf_count_t)(std::numeric_limits<int>::max() / sizeof(short)) /
            sfinfo.channels) {
            sf_close(sndfile);
            throw std::runtime_error("Bad sample count in audio file at " + filePath);
        }

        if (sfinfo.channels == 1) {
            format = MONO16;
        } else if (sfinfo.channels == 2) {
            format = STEREO16;
        } else if (sfinfo.channels == 3) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                format = BFORMAT2D_16;
            } else {
                sf_close(sndfile);
                throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
            }
        } else if (sfinfo.channels == 4) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                format = BFORMAT3D_16;
            } else {
                sf_close(sndfile);
                throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
            }
        } else {
            sf_close(sndfile);
            throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
        }

        sampleRate = sfinfo.samplerate;

        auto *buff = static_cast<short *>(malloc((size_t) (sfinfo.frames * sfinfo.channels) * sizeof(short)));

        sf_count_t num_frames = sf_readf_short(sndfile, buff, sfinfo.frames);
        if (num_frames < 1) {
            free(buff);
            sf_close(sndfile);
            throw std::runtime_error("Failed to read samples from audio data at " + filePath);
        }

        sf_count_t num_bytes = num_frames * sfinfo.channels * sizeof(short);

        buffer.resize(num_bytes);

        std::memcpy(buffer.data(), buff, num_bytes);

        free(buff);
        sf_close(sndfile);
    }

    AudioFormat AudioFile::getFormat() const {
        return format;
    }

    int AudioFile::getSampleRate() const {
        return sampleRate;
    }

    const std::vector<uint8_t> &AudioFile::getBuffer() const {
        return buffer;
    }
}