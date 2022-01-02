#include "common/crypto/sha.hpp"

#include "cryptopp/filters.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"

namespace engine{
    std::string SHA::sha256(const char *data, size_t length) {
        std::string tmp;
        std::string ret;
        CryptoPP::SHA256 hash;
        CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(ret));
        hash.Update((const CryptoPP::byte *) data, length);
        tmp.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte *) &tmp[0]);
        CryptoPP::StringSource(tmp, true, new CryptoPP::Redirector(encoder));
        return ret;
    }

    std::string SHA::sha256(const std::string &data) {
        return sha256(data.data(), data.size());
    }

    std::string SHA::sha256(const std::vector<char> &data) {
        return sha256(data.data(), data.size());
    }
}