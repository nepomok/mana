#include "io/archive/packedarchive.hpp"

#include <filesystem>
#include <sstream>
#include <utility>

#include "crypto/aes.hpp"
#include "crypto/sha.hpp"
#include "compression/gzip.hpp"

#include "json.hpp"
#include "base64.hpp"

static const std::string PAK_FORMAT_VERSION = "00";
static const std::string PAK_HEADER_MAGIC = "\xa9pak\xff" + PAK_FORMAT_VERSION + "\xa9";

class AssetPack {
public:
    explicit AssetPack(engine::PackedArchive::EncryptionKey key)
            : key(std::move(key)), headerStr("{}") {};

    explicit AssetPack(std::istream &stream)
            : stream(&stream) {
        int scope = -1;
        char c;
        stream.seekg(static_cast<std::streamoff>(PAK_HEADER_MAGIC.size()));
        stream.read(&c, 1);
        while (stream.gcount() == 1) {
            headerStr += c;
            if (c == '{')
                if (scope == -1)
                    scope = 1;
                else
                    scope++;
            else if (c == '}')
                scope--;
            if (scope == 0) {
                dataBegin = stream.tellg();
                break;
            }
            stream.read(&c, 1);
        }

        headerStr = base64_decode(static_cast<std::string>(nlohmann::json::parse(headerStr)["data"]));
        headerStr = engine::GZip::decompress(engine::AES::decrypt(key.getValue(), {}, headerStr));
        headerJson = nlohmann::json::parse(headerStr);
    }

    const std::string &getHeaderStr() const {
        return headerStr;
    }

    const nlohmann::json &getHeaderJson() const {
        return headerJson;
    }

    std::istream &getStream() {
        return *stream;
    }

    bool exists(const std::string &path) {
        return headerJson["entries"].find(path) != headerJson["entries"].end();
    }

    std::vector<char> getEntry(const std::string &path) {
        auto entry = headerJson["entries"][path];
        long start = entry["start"];
        long end = entry["end"];
        std::string hash = entry["hash"];
        std::vector<char> ret;
        ret.resize(end - start);
        stream->seekg(dataBegin + start);
        stream->read(ret.data(), ret.size());
        if (stream->gcount() != ret.size()) {
            throw std::runtime_error("Failed to read entry " + path);
        }
        auto dHash = engine::SHA::sha256(ret);
        if (dHash == hash) {
            ret = engine::AES::decrypt(key.getValue(), {}, ret);
            return engine::GZip::decompress(ret);
        } else {
            throw std::runtime_error("Failed to get entry (Corrupted data: Hash Mismatch)");
        }
    }

    void append(const std::string &path, std::vector<char> d) {
        // Offset points to offset for the next asset, assets can only be appended not removed without rebuilding the structure.
        long offset = 0;
        auto it = headerJson.find("offset");
        if (it != headerJson.end())
            offset = *it;

        d = engine::GZip::compress(d);
        d = engine::AES::decrypt(key.getValue(), {}, d);

        auto &entry = headerJson["entries"][path];
        entry["start"] = offset;
        entry["end"] = offset + d.size();
        entry["hash"] = engine::SHA::sha256(d);

        headerJson["offset"] = entry["end"];

        for (auto c: d) {
            data.emplace_back(c);
        }

        headerStr = headerJson.dump();
    }

    std::vector<char> getCombinedData() {
        std::string jsonDump = headerJson.dump();
        nlohmann::json hdrWrap;
        hdrWrap["data"] = base64_encode(engine::AES::encrypt(key.getValue(), {}, engine::GZip::compress(jsonDump)));
        auto hdrStr = PAK_HEADER_MAGIC + hdrWrap.dump();
        std::string dataStr = {data.begin(), data.end()};
        std::vector<char> ret;
        ret.insert(ret.begin(), hdrStr.begin(), hdrStr.end());
        ret.insert(ret.end(), dataStr.begin(), dataStr.end());
        return ret;
    }

private:
    engine::PackedArchive::EncryptionKey key;
    std::string headerStr;
    nlohmann::json headerJson;
    std::istream *stream = nullptr;
    std::streampos dataBegin;

    std::vector<char> data; //Used for output
};

static std::vector<char> readFile(const std::string &path) {
    std::vector<char> ret;

    const size_t step = 100;

    FILE *fp;
    fp = fopen(path.c_str(), "r");

    std::vector<char> fbuf(step);
    auto r = fread(fbuf.data(), 1, fbuf.size(), fp);
    ret.insert(ret.end(), fbuf.begin(), fbuf.begin() + static_cast<long>(r));
    while (r == step) {
        r = fread(fbuf.data(), 1, fbuf.size(), fp);
        ret.insert(ret.end(), fbuf.begin(), fbuf.begin() + r);
    }

    fclose(fp);

    return ret;
}

std::vector<char> engine::PackedArchive::createPack(const std::string &directory, const EncryptionKey &key) {
    AssetPack pack(key);
    for (auto &file: std::filesystem::recursive_directory_iterator(directory)) {
        if (file.is_regular_file()) {
            auto path = file.path().string();
            auto data = readFile(path);
            for (auto &c: directory)
                path.erase(path.begin());
            pack.append(path, data);
        }
    }
    return pack.getCombinedData();
}

engine::PackedArchive::PackedArchive(std::unique_ptr<std::istream> stream, EncryptionKey key)
        : key(std::move(key)), packFileStream(std::move(stream)) {
    pack = std::make_unique<AssetPack>(*packFileStream);
}

engine::PackedArchive::~PackedArchive() = default;

bool engine::PackedArchive::exists(const std::string &path) {
    return pack->exists(path);
}

std::unique_ptr<std::iostream> engine::PackedArchive::open(const std::string &path) {
    std::lock_guard<std::mutex> guard(streamMutex);
    auto entry = pack->getEntry(path);
    auto str = std::string(entry.begin(), entry.end());
    auto ret = std::make_unique<std::stringstream>(str);
    std::noskipws(*ret);
    return std::move(ret);
}
