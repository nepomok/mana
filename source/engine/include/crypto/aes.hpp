#ifndef MANA_AES_HPP
#define MANA_AES_HPP

#include <string>
#include <vector>
#include <array>

namespace engine {
    namespace AES {
        static const int BLOCKSIZE = 128;

        std::string encrypt(const std::string &key, const std::array<char, BLOCKSIZE> &iv, const std::string &plaintext);

        std::string decrypt(const std::string &key, const std::array<char, BLOCKSIZE> &iv, const std::string &ciphertext);

        std::vector<char> encrypt(const std::string &key, const std::array<char, BLOCKSIZE> &iv, const std::vector<char> &plaintext);

        std::vector<char> decrypt(const std::string &key, const std::array<char, BLOCKSIZE> &iv, const std::vector<char> &ciphertext);
    }
}

#endif //MANA_AES_HPP
