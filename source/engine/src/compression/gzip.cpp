#include "compression/gzip.hpp"

#include "cryptopp/filters.h"
#include "cryptopp/gzip.h"
#include "cryptopp/cryptlib.h"

namespace engine {
    std::vector<char> GZip::compress(const char *data, size_t length) {
        std::string compressed;
        CryptoPP::Gzip zipper(new CryptoPP::StringSink(compressed));
        zipper.Put((CryptoPP::byte *) data, length);
        zipper.MessageEnd();
        return {compressed.begin(), compressed.end()};
    }

    std::vector<char> GZip::decompress(const char *data, size_t length) {
        std::string decompressed;
        CryptoPP::Gunzip unzip(new CryptoPP::StringSink(decompressed));
        unzip.Put((CryptoPP::byte *) data, length);
        unzip.MessageEnd();
        return {decompressed.begin(), decompressed.end()};
    }

    std::string GZip::compress(const std::string &data) {
        auto r = compress(data.data(), data.size());
        return {r.begin(), r.end()};
    }

    std::string GZip::decompress(const std::string &data) {
        auto r = decompress(data.data(), data.size());
        return {r.begin(), r.end()};
    }

    std::vector<char> GZip::compress(const std::vector<char> &data) {
        return compress(data.data(), data.size());
    }

    std::vector<char> GZip::decompress(const std::vector<char> &data) {
        return decompress(data.data(), data.size());
    }
}