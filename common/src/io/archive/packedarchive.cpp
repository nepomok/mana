#include "common/io/archive/packedarchive.hpp"

#include <filesystem>
#include <sstream>

#include "json.hpp"
#include "base64.hpp"

static const std::string PAK_HEADER_MAGIC = "#pak#";
static const std::string DEFAULT_HEADER_SRC = PAK_HEADER_MAGIC + std::string(R"({})");

class AssetPack {
public:
    struct Entry {
        char *data;
        size_t size;
    };

    AssetPack()
            : data() {
    }

    explicit AssetPack(const std::vector<char> &bytes) {
        if (bytes.empty())
            return;

        int scope = -1;
        bool readheader = true;
        auto dataBegin = bytes.end();
        for (auto i = PAK_HEADER_MAGIC.size(); i < bytes.size(); i++) {
            auto c = bytes.at(i);
            if (readheader) {
                headerStr += c;
                if (c == '{')
                    if (scope == -1)
                        scope = 1;
                    else
                        scope++;
                else if (c == '}')
                    scope--;
                if (scope == 0)
                    readheader = false;
            } else {
                dataBegin = bytes.begin() + i;
                break;
            }
        }

        auto dataStr = base64_decode({dataBegin, bytes.end()});
        data = {dataStr.begin(), dataStr.end()};

        headerJson = nlohmann::json::parse(headerStr);
    }

    const std::string &getHeaderStr() {
        return headerStr;
    }

    const nlohmann::json &getHeaderJson() {
        return headerJson;
    }

    std::vector<char> &getData() {
        return data;
    }

    void append(const std::string &path, const std::vector<char> &d) {
        // Offset points to offset for the next asset, assets can only be appended not removed without rebuilding the structure.
        long offset = 0;
        auto it = headerJson.find("offset");
        if (it != headerJson.end())
            offset = *it;

        headerJson["entries"][path]["start"] = offset;
        headerJson["entries"][path]["end"] = offset + d.size();
        headerJson["offset"] = headerJson["entries"][path]["end"];

        for (auto c: d) {
            data.emplace_back(c);
        }

        headerStr = headerJson.dump();
    }

    bool exists(const std::string &path) {
        return headerJson["entries"].find(path) != headerJson["entries"].end();
    }

    Entry getEntry(const std::string &path) {
        long start = headerJson["entries"][path]["start"];
        long end = headerJson["entries"][path]["end"];
        Entry ret{};
        ret.size = end - start;
        ret.data = data.data() + start;
        return ret;
    }

    std::vector<char> getCombinedData() {
        auto hdrStr = PAK_HEADER_MAGIC + headerJson.dump();
        auto dataStr = base64_encode({data.begin(), data.end()});
        std::vector<char> ret;
        ret.insert(ret.begin(), hdrStr.begin(), hdrStr.end());
        ret.insert(ret.end(), dataStr.begin(), dataStr.end());
        return ret;
    }

private:
    std::string headerStr;
    nlohmann::json headerJson;
    std::vector<char> data;
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

//TODO: Implement pack encryption
static std::vector<char> encrypt(const engine::PackedArchive::EncryptionKey &key, const std::vector<char> &data) {
    return data;
}

static std::vector<char> decrypt(const engine::PackedArchive::EncryptionKey &key, const std::vector<char> &data) {
    return data;
}

std::vector<char> engine::PackedArchive::createPack(const std::string &directory, const EncryptionKey &key) {
    AssetPack pack({DEFAULT_HEADER_SRC.begin(), DEFAULT_HEADER_SRC.end()});
    for (auto &file: std::filesystem::recursive_directory_iterator(directory)) {
        if (file.is_regular_file()) {
            auto path = file.path().string();
            auto data = readFile(path);
            for (auto &c: directory)
                path.erase(path.begin());
            pack.append(path, data);
        }
    }
    return encrypt(key, pack.getCombinedData());
}

engine::PackedArchive::PackedArchive(std::istream &stream, const EncryptionKey &key)
        :  key(key) {
    packData = decrypt(key, {std::istream_iterator<char>(stream), std::istream_iterator<char>()});
    pack = new AssetPack(packData);
}

engine::PackedArchive::~PackedArchive() {
    delete pack;
}

bool engine::PackedArchive::exists(const std::string &path) {
    return pack->exists(path);
}

std::unique_ptr<std::iostream> engine::PackedArchive::open(const std::string &path) {
    auto entry = pack->getEntry(path);
    std::string str = {entry.data, entry.size};
    auto ret = std::make_unique<std::stringstream>(str);
    std::noskipws(*ret);
    return std::move(ret);
}
