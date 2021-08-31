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

#include "engine/asset/assetimporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "extern/stb_image.h"

#include <sndfile.h>

#include "engine/audio/audioformat.hpp"
#include "engine/io/json/ecs/scenedeserializer.hpp"

namespace mana {
    static Mesh convertMesh(const aiMesh &assMesh) {
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

    static Material convertMaterial(const aiMaterial &assMaterial) {
        Material ret;

        aiColor3D c;
        assMaterial.Get(AI_MATKEY_COLOR_DIFFUSE, c);
        ret.diffuse = {static_cast<uint8_t>(255 * c.r),
                       static_cast<uint8_t>(255 * c.g),
                       static_cast<uint8_t>(255 * c.b),
                       255};

        assMaterial.Get(AI_MATKEY_COLOR_AMBIENT, c);
        ret.ambient = {static_cast<uint8_t>(255 * c.r),
                       static_cast<uint8_t>(255 * c.g),
                       static_cast<uint8_t>(255 * c.b),
                       255};

        assMaterial.Get(AI_MATKEY_COLOR_SPECULAR, c);
        ret.specular = {static_cast<uint8_t>(255 * c.r),
                        static_cast<uint8_t>(255 * c.g),
                        static_cast<uint8_t>(255 * c.b),
                        255};

        assMaterial.Get(AI_MATKEY_SHININESS, ret.shininess);

        return ret;
    }

    static AssetScene readAsset(std::istream &source, const std::string &hint) {
        std::string assetBuffer((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        Assimp::Importer importer;

        const auto *scenePointer = importer.ReadFileFromMemory(assetBuffer.data(),
                                                               assetBuffer.size(),
                                                               aiPostProcessSteps::aiProcess_Triangulate,
                                                               hint.c_str());
        if (scenePointer == nullptr)
            throw std::runtime_error("Failed to read mesh data from memory");

        const auto &scene = dynamic_cast<const aiScene &>(*scenePointer);

        AssetScene ret;

        for (auto i = 0; i < scene.mNumMeshes; i++) {
            const auto &mesh = dynamic_cast<const aiMesh &>(*scene.mMeshes[i]);
            std::string name = mesh.mName.C_Str();

            if (ret.meshes.find(name) != ret.meshes.end())
                throw std::runtime_error("Duplicate mesh name in memory");

            ret.meshes[name] = convertMesh(mesh);
        }

        for (auto i = 0; i < scene.mNumMaterials; i++) {
            auto material = convertMaterial(dynamic_cast<const aiMaterial &>(*scene.mMaterials[i]));
            std::string name;

            scene.mMaterials[i]->Get(AI_MATKEY_NAME, name);

            if (ret.materials.find(name) != ret.materials.end())
                throw std::runtime_error("Duplicate material name in memory");
        }

        return ret;
    }

    static Image<ColorRGBA> readImage(std::istream &source) {
        std::string buffer((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        int width, height, nrChannels;
        stbi_uc *data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(buffer.data()),
                                              buffer.size(),
                                              &width,
                                              &height,
                                              &nrChannels,
                                              4);
        if (data) {
            auto ret = Image<ColorRGBA>(width, height);
            std::memcpy(ret.getData(), data, (width * height) * (sizeof(stbi_uc) * 4));
            stbi_image_free(data);
            return ret;
        } else {
            stbi_image_free(data);
            std::string error = "Failed to load image";
            throw std::runtime_error(error);
        }
    }

    struct LibSndBuffer {
        std::string data;
        size_t pos;
    };

    sf_count_t sf_vio_get_filelen(void *user_data) {
        auto *buffer = reinterpret_cast<LibSndBuffer *>(user_data);
        return buffer->data.size();
    }

    sf_count_t sf_vio_seek(sf_count_t offset, int whence, void *user_data) {
        auto *buffer = reinterpret_cast<LibSndBuffer *>(user_data);
        buffer->pos = offset;
        return buffer->pos;
    }

    sf_count_t sf_vio_read(void *ptr, sf_count_t count, void *user_data) {
        auto *buffer = reinterpret_cast<LibSndBuffer *>(user_data);
        sf_count_t ret;
        for (ret = 0; ret <= count && buffer->pos + ret < buffer->data.size(); ret++) {
            static_cast<char *>(ptr)[ret] = buffer->data.at(buffer->pos + ret);
        }
        buffer->pos += ret;
        return ret;
    }

    sf_count_t sf_vio_write(const void *ptr, sf_count_t count, void *user_data) {
        throw std::runtime_error("VIO Write");
    }

    sf_count_t sf_vio_tell(void *user_data) {
        auto *buffer = reinterpret_cast<LibSndBuffer *>(user_data);
        return buffer->data.size();
    }

    static Audio readAudio(std::istream &source) {
        SF_VIRTUAL_IO virtio;
        virtio.get_filelen = &sf_vio_get_filelen;
        virtio.seek = &sf_vio_seek;
        virtio.read = &sf_vio_read;
        virtio.write = &sf_vio_write;
        virtio.tell = &sf_vio_tell;

        LibSndBuffer buffer{std::string((std::istreambuf_iterator<char>(source)),
                                        std::istreambuf_iterator<char>()),
                            0};
        SF_INFO sfinfo;
        SNDFILE *sndfile = sf_open_virtual(&virtio, SFM_READ, &sfinfo, &buffer);
        if (!sndfile) {
            throw std::runtime_error("Failed to open audio buffer");
        }

        if (sfinfo.frames<1
                          || sfinfo.frames>(sf_count_t)(std::numeric_limits<int>::max() / sizeof(short)) /
            sfinfo.channels) {
            sf_close(sndfile);
            throw std::runtime_error("Bad sample count in audio buffer");
        }

        Audio ret;

        if (sfinfo.channels == 1) {
            ret.format = MONO16;
        } else if (sfinfo.channels == 2) {
            ret.format = STEREO16;
        } else if (sfinfo.channels == 3) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                ret.format = BFORMAT2D_16;
            } else {
                sf_close(sndfile);
                throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
            }
        } else if (sfinfo.channels == 4) {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                ret.format = BFORMAT3D_16;
            } else {
                sf_close(sndfile);
                throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
            }
        } else {
            sf_close(sndfile);
            throw std::runtime_error("Unsupported channel count: " + std::to_string(sfinfo.channels));
        }

        ret.frequency = sfinfo.samplerate;

        auto *buff = static_cast<short *>(malloc((size_t) (sfinfo.frames * sfinfo.channels) * sizeof(short)));

        sf_count_t num_frames = sf_readf_short(sndfile, buff, sfinfo.frames);
        if (num_frames < 1) {
            free(buff);
            sf_close(sndfile);
            throw std::runtime_error("Failed to read samples from audio data");
        }

        sf_count_t num_bytes = num_frames * sfinfo.channels * sizeof(short);

        ret.buffer.resize(num_bytes);

        std::memcpy(ret.buffer.data(), buff, num_bytes);

        free(buff);

        sf_close(sndfile);

        return ret;
    }

    AssetScene AssetImporter::importAssetScene(std::istream &source, const std::string &hint) {
        return readAsset(source, hint);
    }

    Image<ColorRGBA> AssetImporter::importImage(std::istream &source, const std::string &hint) {
        return readImage(source);
    }

    Audio AssetImporter::importAudio(std::istream &source, const std::string &hint) {
        return readAudio(source);
    }

    MonoCppAssembly *AssetImporter::importMonoAssembly(std::istream &source, MonoCppDomain &domain) {
        return domain.loadAssembly(source);
    }

    Scene AssetImporter::importScene(std::istream &source) {
        SceneDeserializer deserializer;
        return deserializer.deserialize(source);
    }
}