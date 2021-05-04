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

#include "engine/resource/file/textfileresource.hpp"

#include "engine/io/file.hpp"

namespace mana {
    TextFileResource::TextFileResource() : filePath() {}

    TextFileResource::TextFileResource(std::string filePath) : filePath(std::move(filePath)) {}

    void TextFileResource::load() {
        if (isLoaded)
            return;
        text = File::readAllText(filePath);
        isLoaded = true;
    }

    void TextFileResource::free() {
        if (!isLoaded)
            return;
        text.clear();
        isLoaded = false;
    }

    const std::string &TextFileResource::getText() {
        if (!isLoaded)
            load();
        return text;
    }
}