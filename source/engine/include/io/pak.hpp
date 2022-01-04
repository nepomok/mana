#ifndef MANA_PAK_HPP
#define MANA_PAK_HPP

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>

#include "crypto/aes.hpp"

namespace engine {
    static const std::string PAK_FORMAT_VERSION = "00";
    static const std::string PAK_HEADER_MAGIC = "\xa9pak\xff" + PAK_FORMAT_VERSION + "\xa9";

    /**
     * The pak file format
     */
    class MANA_EXPORT Pak {
    public:
        struct HeaderEntry {
            size_t offset;
            size_t size;
            std::string hash;
        };

        static std::map<std::string, std::vector<char>> readEntries(const std::string &path, bool recursive = true);

        /**
         * Create a pak buffer from the passed entries and return it.
         *
         * @param entries
         * @return
         */
        static std::vector<char> createPak(const std::map<std::string, std::vector<char>> &entries);

        /**
         * Create a pak buffer from the passed entries and return it.
         * Additionally apply aes encryption to the asset data using the supplied key and initialization vector.
         *
         * @param entries
         * @return
         */
        static std::vector<char> createPak(const std::map<std::string, std::vector<char>> &entries,
                                           const AES::Key &key,
                                           const AES::InitializationVector &iv);

        Pak() = default;

        /**
         * Load a pak buffer from stream.
         *
         * @param s
         * @param key
         * @param iv
         */
        explicit Pak(std::unique_ptr<std::istream> s, AES::Key key = {}, AES::InitializationVector iv = {});

        /**
         * Load the pak entry from the stream, and optionally verify its hash.
         *
         * @param path The path of the entry
         * @param verifyHash If true the data hash is checked against the header hash which ensures data integrity.
         * @return The entry data
         */
        std::vector<char> get(const std::string &path, bool verifyHash = false);

        bool exists(const std::string &path) {
            return entries.find(path) != entries.end();
        }

    private:
        void loadHeader();

        std::unique_ptr<std::istream> stream;
        std::map<std::string, HeaderEntry> entries;
        bool encrypted{};
        AES::Key key{};
        AES::InitializationVector iv{};
    };
}

#endif //MANA_PAK_HPP
