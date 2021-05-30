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

#ifndef MANA_ASSETFILE_HPP
#define MANA_ASSETFILE_HPP

#include <map>

#include "engine/io/file.hpp"
#include "engine/render/mesh.hpp"
#include "engine/render/renderdevice.hpp"
#include "engine/render/3d/material.hpp"

namespace mana {
    class AssetFile : public File {
    public:
        struct MaterialSource {
            std::string name{};

            ColorRGBA diffuseColor{};
            ColorRGBA ambientColor{};
            ColorRGBA specularColor{};
            float shininess{};

            ImageBuffer<ColorRGBA> diffuseTexture;
            ImageBuffer<ColorRGBA> ambientTexture;
            ImageBuffer<ColorRGBA> specularTexture;
            ImageBuffer<ColorRGBA> shininessTexture;

            ImageBuffer<ColorRGBA> normalTexture;

            ImageBuffer<ColorRGBA> emissiveTexture;

            Material *createMaterial(RenderDevice &dev) const {
                auto *ret = new Material();
                ret->name = name;
                ret->diffuseColor = diffuseColor;
                ret->ambientColor = ambientColor;
                ret->specularColor = specularColor;
                ret->shininess = shininess;
                ret->diffuseTexture = dev.createTextureBuffer({});
                ret->ambientTexture = dev.createTextureBuffer({});
                ret->specularTexture = dev.createTextureBuffer({});
                ret->shininessTexture = dev.createTextureBuffer({});
                ret->normalTexture = dev.createTextureBuffer({});
                ret->emissiveTexture = dev.createTextureBuffer({});
                ret->diffuseTexture->upload(diffuseTexture);
                ret->ambientTexture->upload(ambientTexture);
                ret->specularTexture->upload(specularTexture);
                ret->shininessTexture->upload(shininessTexture);
                ret->normalTexture->upload(normalTexture);
                ret->emissiveTexture->upload(emissiveTexture);
                return ret;
            }
        };

        AssetFile();

        explicit AssetFile(const std::string &filePath);

        void open() override;

        void close() override;

        const std::map<std::string, Mesh> &getMeshes();

        const std::map<std::string, MaterialSource> &getMaterials();

    private:
        std::map<std::string, Mesh> meshes;
        std::map<std::string, MaterialSource> materials;
    };
}

#endif //MANA_ASSETFILE_HPP
