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

#ifndef MANA_IMAGEBUFFER_HPP
#define MANA_IMAGEBUFFER_HPP

#include "engine/common/buffer.hpp"
#include "engine/common/color.hpp"
#include "engine/math/rectangle.hpp"

namespace mana {
    template<typename T>
    class ImageBuffer {
    public:
        Buffer<T> buffer;

        ImageBuffer()
                : size(), buffer() {}

        ImageBuffer(int width, int height, const Buffer<T> &buffer) : size(width, height),
                                                                      buffer(buffer) {}

        ImageBuffer(int width, int height)
                : size(width, height), buffer(width * height) {}

        explicit ImageBuffer(Vec2i size)
                : size(size), buffer(size.x * size.y) {}

        ImageBuffer(const ImageBuffer &copy) : size(copy.size), buffer(copy.buffer) {}

        ImageBuffer &operator=(const ImageBuffer &copy) {
            this->width = copy.width;
            this->height = copy.height;
            this->buffer = Buffer<T>(copy.buffer);
            return *this;
        }

        Vec2i getSize() const { return size; }

        int getWidth() const { return size.x; }

        int getHeight() const { return size.y; }

        T getPixel(int x, int y) {
            return buffer[scanLine(y) + x];
        }

        void setPixel(int x, int y, T color) {
            buffer[scanLine(y) + x] = color;
        }

        int scanLine(int y) {
            return y * size.x;
        }

        void blit(const ImageBuffer<T> &source) {
            if (source.size.x != size.x || source.height != size.y) {
                throw std::runtime_error("Invalid blit source size");
            }
            throw std::runtime_error("Not implemented");
        }

        ImageBuffer<T> swapRows() {
            ImageBuffer<T> ret = ImageBuffer<T>(size.x, size.y);
            for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                    ret.setPixel(size.x - 1 - x, y, getPixel(x, y));
                }
            }
            return std::move(ret);
        }

        ImageBuffer<T> swapColumns() {
            ImageBuffer<T> ret = ImageBuffer<T>(size.x, size.y);
            for (int x = 0; x < size.x; x++) {
                for (int y = 0; y < size.y; y++) {
                    ret.setPixel(x, size.y - 1 - y, getPixel(x, y));
                }
            }
            return std::move(ret);
        }

        ImageBuffer<T> slice(const Recti &rect) {
            ImageBuffer<T> ret = ImageBuffer<T>(rect.dimensions.x, rect.dimensions.y);
            for (int x = rect.position.x; x < rect.position.x + rect.dimensions.x; x++) {
                for (int y = rect.position.y; y < rect.position.y + rect.dimensions.y; y++) {
                    ret.setPixel(x - rect.position.x, y - rect.position.y, getPixel(x, y));
                }
            }
            return std::move(ret);
        }

    private:
        Vec2i size;
    };
}

#endif //MANA_IMAGEBUFFER_HPP
