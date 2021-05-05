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

#ifndef MANA_FILE_HPP
#define MANA_FILE_HPP

#include <fstream>

namespace mana {
    class File {
    public:
        static std::string readAllText(const std::string &filepath);

        static void writeAllText(const std::string &filepath, const std::string &text);

        const std::string &getFilePath() { return filePath; }

        void setFilePath(const std::string &fp) {
            if (isOpen)
                close();
            filePath = fp;
        }

        virtual void open() { isOpen = true; }

        virtual void close() { isOpen = false; }

    protected:
        bool isOpen = false;
        std::string filePath;
    };
}

#endif //MANA_FILE_HPP
