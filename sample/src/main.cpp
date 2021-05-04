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

#include "sample0.hpp"

void printUsage() {
    printf("Usage: samples SAMPLE_NUMBER\n");
}

int main(int argc, char *argv[]) {
    int n = 0;
    if (argc == 2) {
        n = std::stoi(argv[1]);
    }

    Game *game;
    switch (n) {
        case 0:
            game = new Sample0();
            break;
        default:
            printUsage();
            return 0;
    }

    int ret = game->loop(OPENGL);
    delete game;
    return ret;
}