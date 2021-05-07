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

#include "engine/io/file.hpp"

#include "engine/ecs/scene.hpp"
#include "engine/resource/resources.hpp"

namespace mana {
    class SceneFile : public File {
    public:
        SceneFile() = default;

        explicit SceneFile(const std::string &filePath);

        void open() override;

        void close() override;

        const std::string &getSceneName();

        const std::string &getSceneResourcesName();

        Scene getScene(Resources &res);

    private:
        std::string sceneName;
        std::string sceneResources;
        std::string sceneJsonSource;
    };
}

#endif //MANA_SCENEFILE_HPP