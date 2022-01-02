#ifndef MANA_LIBRARY_HPP
#define MANA_LIBRARY_HPP

#include <memory>

namespace engine {
    class Library {
    public:
        /**
         * Load a shared library by path.
         *
         * @param path
         * @return
         */
        static std::unique_ptr<Library> load(const std::string &path);

        /**
         * Unload the library, returned symbol addresses become invalid.
         */
        virtual ~Library() = default;

        /**
         * Return the address for the given symbol name.
         *
         * @param name
         * @return
         */
        virtual void *getSymbolAddress(const std::string &name) = 0;

        /**
         * Return the symbol casted to a reference,
         * Not typesafe
         *
         * @tparam T
         * @param name
         * @return
         */
        template<typename T>
        T &getSymbol(const std::string &name) {
            return *((T *) getSymbolAddress(name));
        }
    };
}

#endif //MANA_LIBRARY_HPP
