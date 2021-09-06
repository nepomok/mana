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

#ifndef MANA_ASSETLOADER_HPP
#define MANA_ASSETLOADER_HPP

#include <string>
#include <vector>

#include "engine/io/archive.hpp"

#include "engine/asset/material.hpp"
#include "engine/asset/audio.hpp"
#include "engine/asset/assetbundle.hpp"

#include "engine/async/threadpool.hpp"

namespace mana {
    /**
     * The asset loader defines functions to convert raw data streams into asset bundle objects.
     */
    class AssetLoader {
    public:
        /**
         * Load bundle from 3d, image or audio format.
         *
         * An asset bundle can be described in 3d file formats, image formats, audio formats
         * or custom json format which is handled by the importer.
         *
         * The custom format is json which references other bundles.
         *
         * The data of the referenced bundles is copied into the returned bundle.
         *
         * 3d file formats may also reference other files.
         *
         * Each asset bundle is an isolated copy of data.
         *
         * {
         *  "meshes": [
         *      {
         *          "name": "ExampleMesh",                          <-- The name of the mesh
         *          "bundle": "assets/mesh/example.fbx",            <-- The path to the bundle which contains the mesh.
         *          "asset": "Cube"                                 <-- The name of the mesh inside the bundle.
         *      }
         *  ],
         *  "materials": [
         *      {
         *          "name": "ExampleMaterial",
         *
         *          "bundle": "assets/mesh/example.fbx",
         *          "asset": "Material",
         *
         *          "diffuse": { "r": 255 },
         *          "diffuseTexture": {
         *              "bundle": "assets/textures/example.json",   <-- The path to the bundle containing the texture
         *              "asset": ""
         *          }
         *      }
         *  ],
         *  "textures:" [
         *      {
         *          "name": "ExampleTexture",
         *          "image": "assets/images/example.png",           <-- The path to the texture image
         *                                                          <-- The attributes of the texture
         *      }
         *  ],
         *  "images": [
         *      {
         *          "name": "ExampleImage",
         *          "bundle": "assets/images/example.png",          <-- The path to the bundle which contains the image if no asset name is given a random image is chosen.
         *      }
         *  ]
         * }
         *
         * @param source
         * @param hint
         * @param archive The archive from where to load other bundles referenced in the returned bundle
         * @param pool
         * @return
         */
        static AssetBundle loadBundle(std::istream &source,
                                      const std::string &hint,
                                      Archive &archive,
                                      ThreadPool &pool);
    };
}

#endif //MANA_ASSETLOADER_HPP
