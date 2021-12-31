#include "common/io/archive/packedarchive.hpp"

#include <filesystem>
#include <sstream>
#include <utility>

#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/gzip.h"

#include "json.hpp"
#include "base64.hpp"

static const std::string PAK_HEADER_MAGIC = "#pak#";

static std::vector<char> encrypt(const engine::PackedArchive::EncryptionKey &k, const std::vector<char> &data) {
    std::string plaintext = {data.begin(), data.end()};
    std::string ciphertext;

    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0, CryptoPP::AES::BLOCKSIZE);

    for (auto i = 0; i < k.value.size() && i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
        key[i] = k.value.at(i);
    }

    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char *>(plaintext.c_str()), plaintext.length());
    stfEncryptor.MessageEnd();

    return {ciphertext.begin(), ciphertext.end()};
}

static std::vector<char> decrypt(const engine::PackedArchive::EncryptionKey &k, const std::vector<char> &data) {
    std::string plaintext;
    std::string ciphertext = {data.begin(), data.end()};;

    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];

    memset(key, 0, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0, CryptoPP::AES::BLOCKSIZE);

    for (auto i = 0; i < k.value.size() && i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++) {
        key[i] = k.value.at(i);
    }

    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(plaintext));
    stfDecryptor.Put(reinterpret_cast<const unsigned char *>(ciphertext.c_str()), ciphertext.size());
    stfDecryptor.MessageEnd();

    return {plaintext.begin(), plaintext.end()};
}

static std::vector<char> gzip(const std::vector<char> &data) {
    std::string compressed;
    CryptoPP::Gzip zipper(new CryptoPP::StringSink(compressed));
    zipper.Put((CryptoPP::byte *) data.data(), data.size());
    zipper.MessageEnd();
    return {compressed.begin(), compressed.end()};
}

static std::vector<char> gunzip(const std::vector<char> &data) {
    std::string decompressed;
    CryptoPP::Gunzip unzip(new CryptoPP::StringSink(decompressed));
    unzip.Put((CryptoPP::byte *) data.data(), data.size());
    unzip.MessageEnd();
    return {decompressed.begin(), decompressed.end()};
}

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
        long start = headerJson["entries"][path]["start"];
        long end = headerJson["entries"][path]["end"];
        std::vector<char> ret;
        ret.resize(end - start);
        stream->seekg(dataBegin + start);
        stream->read(ret.data(), ret.size());
        if (stream->gcount() != ret.size()) {
            throw std::runtime_error("Failed to read entry " + path);
        }
        ret = decrypt(key, ret);
        return gunzip(ret);
    }

    void append(const std::string &path, std::vector<char> d) {
        // Offset points to offset for the next asset, assets can only be appended not removed without rebuilding the structure.
        long offset = 0;
        auto it = headerJson.find("offset");
        if (it != headerJson.end())
            offset = *it;

        d = gzip(d);
        d = encrypt(key, d);

        headerJson["entries"][path]["start"] = offset;
        headerJson["entries"][path]["end"] = offset + d.size();
        headerJson["offset"] = headerJson["entries"][path]["end"];

        for (auto c: d) {
            data.emplace_back(c);
        }

        headerStr = headerJson.dump();
    }

    std::vector<char> getCombinedData() {
        auto hdrStr = PAK_HEADER_MAGIC + headerJson.dump();
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
