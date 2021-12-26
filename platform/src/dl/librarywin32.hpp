#ifndef MANA_LIBRARYWIN32_HPP
#define MANA_LIBRARYWIN32_HPP

#include "platform/dl/library.hpp"

#include <string>
#include <windows.h>

namespace engine {
    class LibraryWin32 : public Library {
    public:
        explicit LibraryWin32(const std::string &filePath)
                : filePath(filePath){
            handle = LoadLibrary(TEXT(filePath.c_str()));
        }

        ~LibraryWin32() override {
            FreeLibrary(handle);
        }

        void *getSymbolAddress(const std::string &address) override {
            return GetProcAddress(handle, address.c_str());
        }

        std::string filePath;
        HINSTANCE handle;
    };

    typedef LibraryWin32 LibraryOS;
}

#endif //MANA_LIBRARYWIN32_HPP
