#ifndef MANA_LIBRARYLINUX_HPP
#define MANA_LIBRARYLINUX_HPP

#include "platform/dl/library.hpp"

#include <string>
#include <dlfcn.h>

namespace engine {
    class LibraryLinux : public Library {
    public:
        explicit LibraryLinux(const std::string &filePath)
                : filePath(filePath) {
            handle = dlopen(filePath.c_str(), RTLD_LAZY);
        }

        ~LibraryLinux() override {
            dlclose(handle);
        }

        void *getSymbolAddress(const std::string &address) override {
            return dlsym(handle, address.c_str());
        }

        std::string filePath;
        void *handle;
    };

    typedef LibraryLinux LibraryOS;
}

#endif //MANA_LIBRARYLINUX_HPP
