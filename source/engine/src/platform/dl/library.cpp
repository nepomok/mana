#include "platform/dl/library.hpp"

#ifdef __linux__
#include "platform/dl/librarylinux.hpp"
#else
#ifdef _WIN32
#include "platform/dl/librarywin32.hpp"
#endif
#endif

namespace engine {
    std::unique_ptr <Library> Library::load(const std::string &path) {
        return std::make_unique<LibraryOS>(path);
    }
}