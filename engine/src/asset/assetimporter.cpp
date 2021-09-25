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

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "extern/stb_image.h"

#include <sndfile.h>

#include "engine/audio/audioformat.hpp"
#include "engine/async/threadpool.hpp"

#include "extern/json.hpp"

namespace engine {
    static ColorRGBA convertJsonColor(const nlohmann::json &j) {
        return ColorRGBA(j["r"], j["g"], j["b"], j["a"]);
    }

    static AssetBundle readJsonBundle(const std::string &path, Archive &archive, ThreadPool &pool);

    static void sideLoadBundle(const std::string &bundlePath,
                               ThreadPool &pool,
                               Archive &archive,
                               std::mutex &bundleMutex,
                               std::map<std::string, std::shared_ptr<Task>> bundleTasks,
                               std::map<std::string, AssetBundle> refBundles) {
        bundleMutex.lock();

        auto bundleIterator = refBundles.find(bundlePath);

        if (bundleIterator == refBundles.end()) {
            auto taskIterator = bundleTasks.find(bundlePath);
            if (taskIterator == bundleTasks.end()) {
                bundleTasks[bundlePath] = pool.addTask(
                        [&archive, &bundleMutex, &refBundles, &bundlePath, &pool]() {
                            std::filesystem::path path(bundlePath);

                            std::unique_ptr<std::iostream> stream(archive.open(bundlePath));
                            auto bundle = AssetImporter::import(*stream, path.extension(), &archive);

                            std::lock_guard<std::mutex> guard(bundleMutex);
                            refBundles[bundlePath] = std::move(bundle);
                        });
            }
        }

        bundleMutex.unlock();
    }

    static AssetBundle readJsonBundle(std::istream &stream, Archive &archive, ThreadPool &pool) {
        std::string buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        nlohmann::json j = nlohmann::json::parse(buffer);

        std::mutex bundleMutex;

        std::map<std::string, std::shared_ptr<Task>> bundleTasks;
        std::map<std::string, AssetBundle> refBundles; //The referenced asset bundles by path

        //Begin sideload of all referenced asset bundles
        auto iterator = j.find("meshes");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                sideLoadBundle(element["bundle"], pool, archive, bundleMutex, bundleTasks, refBundles);
            }
        }

        iterator = j.find("images");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                sideLoadBundle(element["bundle"], pool, archive, bundleMutex, bundleTasks, refBundles);
            }
        }

        //Wait for sideload to finish
        for (auto &task : bundleTasks) {
            task.second->wait();
        }

        AssetBundle ret;

        //Load data from json and referenced bundles
        iterator = j.find("meshes");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                std::string name = element["name"];
                std::string bundle = element["bundle"];
                std::string asset = element["asset"];

                ret.meshes[name] = refBundles.at(bundle).getMesh(asset);
            }
        }

        iterator = j.find("materials");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                std::string name = element["name"];

                auto it = element.find("bundle");
                if (it != element.end()) {
                    std::string n = element.value("asset", "");
                    ret.materials[name] = refBundles.at(*it).getMaterial(n);
                } else {
                    Material mat;

                    if (element.find("diffuse") != element.end())
                        mat.diffuse = convertJsonColor(element["diffuse"]);
                    if (element.find("ambient") != element.end())
                        mat.ambient = convertJsonColor(element["ambient"]);
                    if (element.find("specular") != element.end())
                        mat.specular = convertJsonColor(element["specular"]);
                    if (element.find("emissive") != element.end())
                        mat.emissive = convertJsonColor(element["emissive"]);
                    if (element.find("shininess") != element.end())
                        mat.shininess = element["shininess"];

                    if (element.find("diffuseTexture") != element.end())
                        mat.diffuseTexture = {element["diffuseTexture"]["bundle"], element["diffuseTexture"]["asset"]};
                    if (element.find("ambientTexture") != element.end())
                        mat.ambientTexture = {element["ambientTexture"]["bundle"], element["ambientTexture"]["asset"]};
                    if (element.find("specularTexture") != element.end())
                        mat.specularTexture = {element["specularTexture"]["bundle"],
                                               element["specularTexture"]["asset"]};
                    if (element.find("emissiveTexture") != element.end())
                        mat.emissiveTexture = {element["emissiveTexture"]["bundle"],
                                               element["emissiveTexture"]["asset"]};
                    if (element.find("shininessTexture") != element.end())
                        mat.shininessTexture = {element["shininessTexture"]["bundle"],
                                                element["shininessTexture"]["asset"]};
                    if (element.find("normalTexture") != element.end())
                        mat.normalTexture = {element["normalTexture"]["bundle"], element["normalTexture"]["asset"]};

                    ret.materials[name] = mat;
                }
            }
        }

        iterator = j.find("textures");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                std::string name = element["name"];

                Texture texture;
                texture.image = {element["image"]["bundle"], element["image"]["asset"]};

                //TODO: Parse json texture attributes

                texture.attributes.generateMipmap = true;

                ret.textures[name] = texture;
            }
        }

        iterator = j.find("images");
        if (iterator != j.end()) {
            for (auto &element : *iterator) {
                std::string name = element["name"];
                std::string bundle = element["bundle"];
                std::string asset = element.value("asset", "");

                ret.images[name] = refBundles.at(bundle).getImage(asset);
            }
        }

        return ret;
    }

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
            Vec3f tangent{};
            Vec3f bitangent{};

            if (assMesh.mNormals != nullptr) {
                const auto &n = dynamic_cast<const aiVector3D &>(assMesh.mNormals[y]);
                norm = {n.x, n.y, n.z};
                const auto &t = dynamic_cast<const aiVector3D &>(assMesh.mTangents[y]);
                tangent = {t.x, t.y, t.z};
                const auto &bt = dynamic_cast<const aiVector3D &>(assMesh.mBitangents[y]);
                bitangent = {bt.x, bt.y, bt.z};
            }

            if (assMesh.mTextureCoords[0] != nullptr) {
                const auto &t = dynamic_cast<const aiVector3D &>(assMesh.mTextureCoords[0][y]);
                uv = {t.x, t.y};
            }

            ret.vertices.emplace_back(Vertex(pos, norm, uv, tangent, bitangent));
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

    static AssetBundle readAsset(const std::string &assetBuffer, const std::string &hint, Archive *archive) {
        //TODO: Implement assimp IOSystem pointing to archive

        Assimp::Importer importer;

        const auto *scenePointer = importer.ReadFileFromMemory(assetBuffer.data(),
                                                               assetBuffer.size(),
                                                               aiPostProcessSteps::aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs,
                                                               hint.c_str());
        if (scenePointer == nullptr)
            throw std::runtime_error("Failed to read mesh data from memory");

        const auto &scene = dynamic_cast<const aiScene &>(*scenePointer);

        AssetBundle ret;

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

            scene.mMaterials[i]->Get<std::string>(AI_MATKEY_NAME, name);

            if (ret.materials.find(name) != ret.materials.end())
                continue; //TODO: Handle bundle entries without a name or sharing a name

            ret.materials[name] = material;
        }

        return ret;
    }

    static Image<ColorRGBA> readImage(const std::string &buffer) {
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

    static Audio readAudio(const std::string &buf) {
        SF_VIRTUAL_IO virtio;
        virtio.get_filelen = &sf_vio_get_filelen;
        virtio.seek = &sf_vio_seek;
        virtio.read = &sf_vio_read;
        virtio.write = &sf_vio_write;
        virtio.tell = &sf_vio_tell;

        LibSndBuffer buffer{buf, 0};
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

    AssetBundle AssetImporter::import(std::istream &stream, const std::string &hint, Archive *archive) {
        if (hint.empty()) {
            //Try to read source as image
            std::string buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
            int x, y, n;
            if (stbi_info_from_memory(reinterpret_cast<const stbi_uc *>(buffer.data()),
                                      buffer.size(),
                                      &x,
                                      &y,
                                      &n) == 1) {
                //Source is image
                AssetBundle ret;
                ret.images["0"] = readImage(buffer);
                return ret;
            }

            //Try to read source as json
            try {
                if (archive == nullptr)
                    throw std::runtime_error("Null archive while parsing json");

                return readJsonBundle(stream, *archive, ThreadPool::pool);
            } catch (std::exception &e) {}

            //Try to read source as asset
            try {
                return readAsset(buffer, hint, archive);
            } catch (std::exception &e) {}

            //Try to read source as audio
            auto audio = readAudio(buffer);
            AssetBundle ret;
            ret.audio["0"] = audio;

            return ret;
        } else {
            if (hint == ".json") {
                //Try to read source as json
                return readJsonBundle(stream, *archive, ThreadPool::pool);
            } else {
                Assimp::Importer importer;
                if (importer.IsExtensionSupported(hint)) {
                    //Try to read source as asset
                    std::string buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
                    return readAsset(buffer, hint, archive);
                } else {
                    //Try to read source as image
                    std::string buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
                    int x, y, n;
                    if (stbi_info_from_memory(reinterpret_cast<const stbi_uc *>(buffer.data()),
                                              buffer.size(),
                                              &x,
                                              &y,
                                              &n) == 1) {
                        AssetBundle ret;
                        ret.images["0"] = readImage(buffer);
                        return ret;
                    }

                    //Try to read source as audio
                    auto audio = readAudio(buffer);
                    AssetBundle ret;
                    ret.audio["0"] = audio;
                    return ret;
                }
            }
        }
    }

    AssetBundle AssetImporter::import(const std::string &path, Archive &archive) {
        auto *stream = archive.open(path);
        auto ret = import(*stream, std::filesystem::path(path).extension(), &archive);
        delete stream;
        return ret;
    }
}