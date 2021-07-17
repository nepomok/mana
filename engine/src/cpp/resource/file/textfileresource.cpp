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

    TextFileResource::~TextFileResource() {
        TextFileResource::free();
    }

    void TextFileResource::load() {
        if (loaded)
            return;
        text = File::readAllText(filePath);
        loaded = true;
    }

    void TextFileResource::free() {
        if (!loaded)
            return;
        text.clear();
        loaded = false;
    }

    std::string &TextFileResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return text;
    }

    bool TextFileResource::isLoaded() {
        return loaded;
    }

    bool TextFileResource::supportAsync() {
        return false;
    }

    std::string &TextFileResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}