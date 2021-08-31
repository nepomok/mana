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

#ifndef MANA_ASSETIMPORTER_HPP
#define MANA_ASSETIMPORTER_HPP

#include <string>
#include <vector>

#include "engine/ecs/scene.hpp"

#include "engine/io/archive.hpp"

#include "engine/asset/material.hpp"
#include "engine/asset/audio.hpp"
#include "engine/asset/assetscene.hpp"

#include "engine/script/script.hpp"

#include "engine/script/mono/monocppassembly.hpp"
#include "engine/script/mono/monocppdomain.hpp"

namespace mana {
    /**
     * The asset importer defines import functions which convert raw data streams into objects useable by the engine.
     */
    class AssetImporter {
    public:
        static AssetScene importAssetScene(std::istream &source, const std::string &hint);

        static Image<ColorRGBA> importImage(std::istream &source, const std::string &hint);

        static Audio importAudio(std::istream &source, const std::string &hint);

        static MonoCppAssembly *importMonoAssembly(std::istream &source, MonoCppDomain &domain);

        /**
         * Import scene in json format
        * {
        *  {
        *      "name": "testnode",
        *      "components": [
        *          {
        *              "type": "mesh",
        *              "path": "asset/test.fbx",
        *              "name": "TestMesh"
        *          },
        *          {
        *              "type": "material",
        *
        *              "path": "asset/test.fbx",
        *              "name": "TestMaterial"
        *
         *             "path": "asset/testmaterial.json"
         *             "name": ""
        *          },
        *          {
        *              "type": "script",
         *             "runtime": "mono",
        *              "path": "asset/test.dll",
         *             "nameSpace": "",
        *              "className": "TestScript"
        *          }
        *      ]
        *  }
        * }
        * @param source
        * @return
        */
        static Scene importScene(std::istream &source);
    };
}

#endif //MANA_ASSETIMPORTER_HPP
