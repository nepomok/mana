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

#include "engine/io/assetfile.hpp"

#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mana {
    Mesh convertMesh(const aiMesh &assMesh) {
        Mesh ret;
        ret.primitive = Mesh::TRI;
        ret.indexed = true;
        for (int y = 0; y < assMesh.mNumFaces; y++) {
            const auto &face = dynamic_cast<const aiFace &>(assMesh.mFaces[y]);
            if (face.mNumIndices != 3)
                throw std::runtime_error("Mesh triangulation failed");
            for (int z = 0; z < face.mNumIndices; z++) {
                ret.indices.emplace_back(face.mIndices[z]);
            }
        }

        for (int y = 0; y < assMesh.mNumVertices; y++) {
            const auto &p = dynamic_cast<const aiVector3D &>(assMesh.mVertices[y]);

            Vec3f pos{p.x, p.y, p.z};
            Vec3f norm{};
            Vec2f uv{};

            if (assMesh.mNormals != nullptr) {
                const auto &n = dynamic_cast<const aiVector3D &>(assMesh.mNormals[y]);
                norm = {n.x, n.y, n.z};
            }

            if (assMesh.mTextureCoords[0] != nullptr) {
                const auto &t = dynamic_cast<const aiVector3D &>(assMesh.mTextureCoords[0][y]);
                uv = {t.x, -t.y};
            }

            ret.vertices.emplace_back(Vertex(pos, norm, uv));
        }

        return ret;
    }

    std::map<std::string, Mesh> readMeshFromFile(const std::string &filePath) {
        Assimp::Importer importer;
        std::map<std::string, Mesh> ret;
        const auto *scenePointer = importer.ReadFile(filePath, aiPostProcessSteps::aiProcess_Triangulate);
        if (scenePointer == nullptr)
            throw std::runtime_error("Failed to open mesh file at " + filePath);
        const auto &scene = dynamic_cast<const aiScene &>(*scenePointer);
        for (auto i = 0; i < scene.mNumMeshes; i++) {
            const auto &aimesh = dynamic_cast<const aiMesh &>(*scene.mMeshes[0]);
            std::string meshname = aimesh.mName.C_Str();
            if (ret.find(meshname) != ret.end())
                throw std::runtime_error("Duplicate mesh name in file " + filePath);
            ret[aimesh.mName.C_Str()] = convertMesh(aimesh);
        }
        return ret;
    }

    AssetFile::AssetFile() = default;

    AssetFile::AssetFile(const std::string &filePath) : meshes(readMeshFromFile(filePath)) {
        this->filePath = filePath;
    }

    void AssetFile::open() {
        meshes = readMeshFromFile(filePath);
        File::open();
    }

    void AssetFile::close() {
        meshes.clear();
        File::close();
    }

    const std::map<std::string, Mesh> &AssetFile::getMeshData() {
        return meshes;
    }
}
