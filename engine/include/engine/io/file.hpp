#ifndef MANA_FILE_HPP
#define MANA_FILE_HPP

#include <fstream>

#include "engine/io/buffer.hpp"

namespace mana {
    class File {
    public:
        File();

        File(std::string filepath);

        void open(std::string filepath);

        void close();

        Buff8 readBuffer();

        std::iostream &getStream();

    private:
        std::iostream stream;
    };
}

#endif //MANA_FILE_HPP
