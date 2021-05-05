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

#ifndef MANA_JSONRESOURCEFILE_HPP
#define MANA_JSONRESOURCEFILE_HPP

#include "engine/io/file.hpp"

#include "engine/resource/resources.hpp"

#include "engine/render/renderallocator.hpp"
#include "engine/script/mono/monocppdomain.hpp"

namespace mana {
    /**
     * A json resource file.
     */
    class JsonResourceFile : public File {
    public:
        explicit JsonResourceFile(const std::string &filePath);

        void open() override;

        void close() override;

        Resources* getResources(RenderAllocator &allocator, MonoCppDomain &monoRuntime);

    private:
        std::string fileContents;
    };
}

#endif //MANA_JSONRESOURCEFILE_HPP
