/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_PACKEDARCHIVE_HPP
#define MANA_PACKEDARCHIVE_HPP

#include <fstream>
#include <vector>
#include <mutex>

#include "io/archive.hpp"

class AssetPack;

namespace engine {
    class PackedArchive : public Archive {
    public:
        struct EncryptionKey {
            EncryptionKey() = default;

            explicit EncryptionKey(const std::string &key)
                    : value(std::move(key)) {}

            //TODO: Implement key scrambling to make binary static analysis harder
            static inline std::string getScrambled(const std::string &string) {
                return string;
            }

            static inline std::string getUnscrambled(const std::string &string) {
                return string;
            }

            std::string getUnscrambled() const {
                return getUnscrambled(value);
            }

            const std::string &getValue() {
                return value;
            }

        private:
            std::string value;
        };

        static std::vector<char> createPack(const std::string &directory,
                                            const engine::PackedArchive::EncryptionKey &key = {});

        PackedArchive() = default;

        explicit PackedArchive(std::unique_ptr<std::istream> stream, EncryptionKey key = {});

        ~PackedArchive() override;

        bool exists(const std::string &path) override;

        std::unique_ptr<std::iostream> open(const std::string &path) override;

    private:
        EncryptionKey key;

        std::mutex streamMutex;
        std::unique_ptr<std::istream> packFileStream;
        std::unique_ptr<AssetPack> pack;
    };
}

#endif //MANA_PACKEDARCHIVE_HPP
