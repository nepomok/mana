/**
 *  Mana - 3D Engine
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

#ifndef MANA_BUFFER_HPP
#define MANA_BUFFER_HPP

#include <stdexcept>
#include <cstring>

namespace mana {
    /**
     * Fixed length buffer of T objects.
     * Users can expect the buffer to always be of the size length.
     * Users shall not delete the data themselves, deletion of the data is done by the destructor.
     * Users can move and copy buffer objects.
     *
     * When a buffer is moved the original buffer length becomes 0 and the original pointer is set to nullptr.
     *
     * @tparam T The type of the buffer objects.
     */
    template<typename T>
    class Buffer {
    public:
        Buffer() : length(0), data(nullptr) {}

        Buffer(Buffer &&orig) {
            length = orig.length;
            data = orig.data;
            orig.length = 0;
            orig.data = nullptr;
        }

        Buffer(const Buffer &copy) {
            length = copy.length;
            data = new T[copy.length];
            for (int i = 0; i < copy.length; i++)
                data[i] = copy.data[i];
        }

        Buffer &operator=(const Buffer &copy) {
            length = copy.length;
            data = new T[copy.length];
            for (int i = 0; i < copy.length; i++)
                data[i] = copy.data[i];
            return *this;
        }

        Buffer(size_t length, T *data) : length(length), data(data) {}

        explicit Buffer(size_t length)
                : length(length), data(new T[length]) {}

        ~Buffer() {
            delete[] data;
        }

        int getLength() const {
            return length;
        }

        T *getData() const {
            return data;
        }

        T &operator[](size_t index) const {
            if (index >= length)
                throw std::runtime_error("Buffer array index out of bounds");
            return data[index];
        }

    private:
        size_t length;
        T *data;
    };

    typedef Buffer<uint8_t> Buff8;
    typedef Buffer<uint16_t> Buff16;
    typedef Buffer<uint32_t> Buff32;
    typedef Buffer<uint64_t> Buff64;
}

#endif //MANA_BUFFER_HPP
