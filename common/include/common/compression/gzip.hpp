#ifndef MANA_GZIP_HPP
#define MANA_GZIP_HPP

#include <string>
#include <vector>

namespace engine {
    namespace GZip {
        std::vector<char> compress(const char *data, size_t length);

        std::vector<char> decompress(const char *data, size_t length);

        std::vector<char> compress(const std::vector<char> &data);

        std::vector<char> decompress(const std::vector<char> &data);

        std::string compress(const std::string &data);

        std::string decompress(const std::string &data);
    }
}

#endif //MANA_GZIP_HPP
