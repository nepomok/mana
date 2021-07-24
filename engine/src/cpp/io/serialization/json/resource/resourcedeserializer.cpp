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

#include "engine/io/json/resource/resourcedeserializer.hpp"

#include "engine/resource/file/assetfileresource.hpp"
#include "engine/resource/file/textfileresource.hpp"
#include "engine/resource/file/imagefileresource.hpp"

#include "engine/resource/render/meshbufferresource.hpp"
#include "engine/resource/render/shaderresource.hpp"
#include "engine/resource/render/texturebufferresource.hpp"
#include "engine/resource/render/materialresource.hpp"

#include "engine/resource/script/monoscriptresource.hpp"

#include "extern/json.hpp"

namespace mana {
    TextureBuffer::TextureType parseTextureType(const std::string &str) {
        if (str == "texture2d")
            return TextureBuffer::TEXTURE_2D;
        else if (str == "cubemap")
            return TextureBuffer::TEXTURE_CUBE_MAP;
        else
            throw std::runtime_error("Invalid texture type " + str);
    }

    TextureBuffer::ColorFormat parseColorFormat(const std::string &str) {
        if (str == "rgba")
            return TextureBuffer::RGBA;
        else if (str == "rgb")
            return TextureBuffer::RGB;
        else
            throw std::runtime_error("Invalid color format " + str);
    }

    TextureBuffer::TextureWrapping parseTextureWrapping(const std::string &str) {
        if (str == "repeat")
            return TextureBuffer::REPEAT;
        else if (str == "mirrored_repeat")
            return TextureBuffer::MIRRORED_REPEAT;
        else if (str == "clamp_to_edge")
            return TextureBuffer::CLAMP_TO_EDGE;
        else if (str == "clamp_to_border")
            return TextureBuffer::CLAMP_TO_BORDER;
        else
            throw std::runtime_error("Invalid texture wrapping " + str);
    }

    TextureBuffer::TextureFiltering parseTextureFiltering(const std::string &str) {
        if (str == "nearest")
            return TextureBuffer::NEAREST;
        else if (str == "linear")
            return TextureBuffer::LINEAR;
        else
            throw std::runtime_error("Invalid texture filtering " + str);
    }

    TextureBuffer::MipMapFiltering parseMipMapFiltering(const std::string &str) {
        if (str == "nearest_mipmap_nearest")
            return TextureBuffer::NEAREST_MIPMAP_NEAREST;
        else if (str == "linear_mipmap_nearest")
            return TextureBuffer::LINEAR_MIPMAP_NEAREST;
        else if (str == "nearest_mipmap_linear")
            return TextureBuffer::NEAREST_MIPMAP_LINEAR;
        else if (str == "linear_mipmap_linear")
            return TextureBuffer::LINEAR_MIPMAP_LINEAR;
        else
            throw std::runtime_error("Invalid mipmap filtering " + str);
    }

    Transform parseTransform(const nlohmann::json &component) {
        Transform ret;

        ret.position.x = component["position"]["x"];
        ret.position.y = component["position"]["y"];
        ret.position.z = component["position"]["z"];

        ret.rotation.x = component["rotation"]["x"];
        ret.rotation.y = component["rotation"]["y"];
        ret.rotation.z = component["rotation"]["z"];

        ret.scale.x = component["scale"]["x"];
        ret.scale.y = component["scale"]["y"];
        ret.scale.z = component["scale"]["z"];

        return ret;
    }

    TextFileResource *parseTextFile(const nlohmann::json &j) {
        return new TextFileResource(j["filePath"]);
    }

    ImageFileResource *parseImageFile(const nlohmann::json &j) {
        return new ImageFileResource(j["filePath"]);
    }

    MonoScriptResource *parseMonoScript(const nlohmann::json &j, MonoCppDomain &monoRuntime) {
        return new MonoScriptResource(monoRuntime, j["assemblyFileName"], j["nameSpace"], j["className"]);
    }

    Resource<ShaderProgram> *parseShader(const nlohmann::json &j, ResourceManager *res, RenderDevice &device) {
        return new ShaderResource(device,
                                  j["vertexShaderResourceName"],
                                  j["fragmentShaderResourceName"],
                                  res);
    }

    MeshBufferResource *parseMeshBuffer(const nlohmann::json &j, ResourceManager *res, RenderDevice &device) {
        if (j["instanced"]) {
            std::vector<Transform> offsets;
            for (auto &t : j["instanceOffsets"]) {
                offsets.emplace_back(parseTransform(t));
            }
            return new MeshBufferResource(device, j["assetResourceName"], j["meshName"], res, offsets);
        } else {
            return new MeshBufferResource(device, j["assetResourceName"], j["meshName"], res);
        }
    }

    TextureBufferResource *parseTexture(const nlohmann::json &j, ResourceManager *res, RenderDevice &device) {
        TextureBuffer::Attributes attr;
        attr.textureType = parseTextureType(j["textureType"]);
        attr.format = parseColorFormat(j["format"]);
        attr.wrapping = parseTextureWrapping(j["wrapping"]);
        attr.filterMin = parseTextureFiltering(j["filterMin"]);
        attr.filterMag = parseTextureFiltering(j["filterMag"]);
        attr.generateMipmap = j["generateMipmap"];
        attr.mipmapFilter = parseMipMapFiltering(j["mipmapFilter"]);
        return new TextureBufferResource(device, j["imageResourceName"], res, attr);
    }

    ColorRGBA parseColor(const nlohmann::json &j) {
        return {j.at("r"), j.at("g"), j.at("b"), j["a"]};
    }

    MaterialResource *parseMaterial(const nlohmann::json &j, ResourceManager *res, RenderDevice &device) {
        return new MaterialResource(&device,
                                    res,
                                    j.contains("diffuse") ? parseColor(j.at("diffuse")) : ColorRGBA(),
                                    j.contains("ambient") ? parseColor(j.at("ambient")) : ColorRGBA(),
                                    j.contains("specular") ? parseColor(j.at("specular")) : ColorRGBA(),
                                    j.contains("emissive") ? parseColor(j.at("emissive")) : ColorRGBA(),
                                    j.contains("shininess") ? (float) j.at("shininess") : 32,
                                    j.value("diffuseTexture",""),
                                    j.value("ambientTexture", ""),
                                    j.value("specularTexture", ""),
                                    j.value("emissiveTexture", ""),
                                    j.value("shininessTexture", ""),
                                    j.value("normalTexture", ""));
    }

    AssetFileResource *parseAssetFile(const nlohmann::json &j) {
        return new AssetFileResource(AssetFile(j["filePath"]));
    }

    ResourceDeserializer::ResourceDeserializer()
            : manager(nullptr),
              device(nullptr),
              monoRuntime(nullptr) {}

    ResourceDeserializer::ResourceDeserializer(ResourceManager &manager,
                                               RenderDevice &device,
                                               MonoCppDomain &monoRuntime)
            : manager(&manager),
              device(&device),
              monoRuntime(&monoRuntime) {}

    ResourceBase *ResourceDeserializer::deserialize(std::istream &stream) {
        assert(manager != nullptr);
        assert(device != nullptr);
        assert(monoRuntime != nullptr);

        nlohmann::json j;
        stream >> j;

        std::string type = j.at("type");

        if (type == "textfile")
            return parseTextFile(j);
        else if (type == "imagefile")
            return parseImageFile(j);
        else if (type == "assetfile")
            return parseAssetFile(j);
        else if (type == "material")
            return parseMaterial(j, manager, *device);
        else if (type == "monoscript")
            return parseMonoScript(j, *monoRuntime);
        else if (type == "shader")
            return parseShader(j, manager, *device);
        else if (type == "meshbuffer")
            return parseMeshBuffer(j, manager, *device);
        else if (type == "texture")
            return parseTexture(j, manager, *device);
        else
            throw std::runtime_error("Invalid resource type " + type);
    }
}