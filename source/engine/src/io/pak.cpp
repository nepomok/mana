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

#include "io/pak.hpp"

#include <utility>
#include <filesystem>

#include "json.hpp"
#include "base64.hpp"

#include "io/readfile.hpp"
#include "compression/gzip.hpp"
#include "crypto/sha.hpp"

namespace engine {
    std::map<std::string, std::vector<char>> Pak::readEntries(const std::string &directory, bool recursive) {
        std::map<std::string, std::vector<char>> ret;
        for (auto &file: std::filesystem::recursive_directory_iterator(directory)) {
            if (file.is_regular_file()) {
                auto path = "/" + std::filesystem::relative(file.path(), directory).string();
                auto data = readFile(file.path().string());
                ret[path] = data;
            }
        }
        return ret;
    }

    static std::vector<char> createPakInternal(const std::map<std::string, std::vector<char>> &entries,
                                               bool encrypt = false,
                                               const AES::Key &key = {},
                                               const AES::InitializationVector &iv = {}) {
        std::vector<char> data;
        size_t currentOffset = 0;

        std::map<std::string, Pak::HeaderEntry> headerEntries;
        for (auto &pair: entries) {
            auto d = GZip::compress(pair.second);

            if (encrypt) {
                d = AES::encrypt(key, iv, d);
            }

            headerEntries[pair.first].offset = currentOffset;
            headerEntries[pair.first].size = d.size();
            headerEntries[pair.first].hash = SHA::sha256(pair.second);

            currentOffset += d.size();

            data.insert(data.begin() + static_cast<long>(data.size()), d.begin(), d.end());
        }

        nlohmann::json headerJson;
        for (auto &pair: headerEntries) {
            auto &element = headerJson["entries"][pair.first];
            element["offset"] = pair.second.offset;
            element["size"] = pair.second.size;
            element["hash"] = pair.second.hash;
        }

        auto headerStr = headerJson.dump();
        headerStr = GZip::compress(headerStr);

        if (encrypt) {
            headerStr = AES::encrypt(key, iv, headerStr);
        }

        nlohmann::json outHeaderJson;
        outHeaderJson["encrypted"] = encrypt;
        outHeaderJson["data"] = base64_encode(headerStr);

        auto outHeaderStr = outHeaderJson.dump();

        // Prepend the header
        data.insert(data.begin(), outHeaderStr.begin(), outHeaderStr.end());
        // Prepend the magic value
        data.insert(data.begin(), PAK_HEADER_MAGIC.begin(), PAK_HEADER_MAGIC.end());

        return data;
    }

    std::vector<char> Pak::createPak(const std::map<std::string, std::vector<char>> &entries) {
        return createPakInternal(entries);
    }

    std::vector<char> Pak::createPak(const std::map<std::string, std::vector<char>> &entries,
                                     const AES::Key &key,
                                     const AES::InitializationVector &iv) {
        return createPakInternal(entries, true, key, iv);
    }

    Pak::Pak(std::unique_ptr<std::istream> stream, AES::Key key, AES::InitializationVector iv)
            : stream(std::move(stream)),
              key(std::move(key)),
              iv(iv) {
        loadHeader();
    }

    std::vector<char> Pak::get(const std::string &path, bool verifyHash) {
        auto hEntry = entries.at(path);

        std::vector<char> ret(hEntry.size);
        stream->seekg(static_cast<std::streamoff>(hEntry.offset));
        stream->read(ret.data(), static_cast<std::streamoff>(hEntry.size));

        if (encrypted) {
            ret = AES::decrypt(key, iv, ret);
        }

        ret = GZip::decompress(ret);

        if (verifyHash) {
            auto hash = SHA::sha256(ret);
            if (hEntry.hash != hash) {
                throw std::runtime_error("Pak entry data hash mismatch");
            }
        }

        return ret;
    }

    void Pak::loadHeader() {
        char c;
        stream->seekg(static_cast<std::streamoff>(PAK_HEADER_MAGIC.size()));
        stream->read(&c, 1);

        size_t dataBegin;
        std::string headerStr;
        int scope = -1;
        while (stream->gcount() == 1) {
            headerStr += c;
            if (c == '{')
                if (scope == -1)
                    scope = 1;
                else
                    scope++;
            else if (c == '}')
                scope--;
            if (scope == 0) {
                dataBegin = stream->tellg();
                break;
            }
            stream->read(&c, 1);
        }

        auto headerWrap = nlohmann::json::parse(headerStr);

        encrypted = headerWrap["encrypted"];

        headerStr = base64_decode(static_cast<std::string>(headerWrap["data"]));

        if (encrypted) {
            try {
                headerStr = engine::AES::decrypt(key, {}, headerStr);
            } catch (const std::exception &e) {
                std::string error = "Failed to decrypt pak header (Wrong Key?): " + std::string(e.what());
                throw std::runtime_error(error);
            }
        }

        headerStr = engine::GZip::decompress(headerStr);

        auto headerJson = nlohmann::json::parse(headerStr);
        for (auto &pair: headerJson.value<std::map<std::string, nlohmann::json>>("entries", {})) {
            auto &path = pair.first;
            auto &entry = pair.second;
            size_t offset = entry["offset"];
            size_t size = entry["size"];
            std::string hash = entry["hash"];
            entries[path] = {dataBegin + offset, size, hash};
        }
    }
}