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

#ifndef MANA_SCENEFILE_HPP
#define MANA_SCENEFILE_HPP

#include "engine/ecs/scene.hpp"
#include "engine/render/renderallocator.hpp"
#include "engine/io/file.hpp"
#include "engine/script/mono/monocppruntime.hpp"

namespace mana {
    class SceneFile : public File {
    public:
        SceneFile();

        explicit SceneFile(const std::string &filepath);

        void open(const std::string &filePath) override;

        void close() override;

        Scene loadScene(RenderAllocator &alloc, MonoCppRuntime &monoRuntime);

    private:
        std::string fileText;
    };
}

#endif //MANA_SCENEFILE_HPP
