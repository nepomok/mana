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

#ifndef MANA_IMAGEFILERESOURCE_HPP
#define MANA_IMAGEFILERESOURCE_HPP

#include "engine/resource/resource.hpp"
#include "engine/render/imagebuffer.hpp"

namespace mana {
    class ImageFileResource : public Resource<ImageBuffer<ColorRGBA>> {
    public:
        ImageFileResource();

        explicit ImageFileResource(std::string filePath);

        ~ImageFileResource() override = default;

        void load() override;

        void free() override;

        const ImageBuffer <ColorRGBA> &get() override;

    private:
        std::string filePath;
        ImageBuffer<ColorRGBA> buffer;
    };
}

#endif //MANA_IMAGEFILERESOURCE_HPP
