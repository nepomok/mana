#ifndef MANA_SHA_HPP
#define MANA_SHA_HPP

#include <string>
#include <vector>

namespace engine {
    namespace SHA {
        std::string sha256(const char *data, size_t length);

        std::string sha256(const std::string &data);

        std::string sha256(const std::vector<char> &data);
    }
}

#endif //MANA_SHA_HPP
