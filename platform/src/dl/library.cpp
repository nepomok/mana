#include "platform/dl/library.hpp"

#ifdef __linux__
#include "dl/librarylinux.hpp"
#else
#ifdef _WIN32
#include "dl/librarywin32.hpp"
#endif
#endif

namespace engine {
    std::unique_ptr <Library> Library::load(const std::string &path) {
        return std::make_unique<LibraryOS>(path);
    }
}