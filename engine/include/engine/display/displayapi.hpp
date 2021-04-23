/**
 *  Mana - 3D Engine
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

#ifndef MANA_DISPLAYAPI_HPP
#define MANA_DISPLAYAPI_HPP

#include <set>

#include "engine/display/window.hpp"
#include "engine/display/windowattributes.hpp"
#include "engine/render/graphicsapi.hpp"

namespace mana {
    /**
     * The Display api is responsible for instantiating windows for a specific graphics api
     * and retrieving display related information such as monitors.
     */
    class DisplayAPI {
    public:
        static Monitor *getPrimaryMonitor();

        static std::set<Monitor *> getMonitors();

        /**
         * Create a window with default attributes.
         *
         * @return
         */
        static Window *createWindow(GraphicsApi api);

        /**
         * Create a window using the supplied attributes.
         *
         * @param title
         * @param size
         * @param attributes
         * @return
         */
        static Window *createWindow(GraphicsApi api, std::string title, Vec2i size, WindowAttributes attributes);

        /**
         * Create a fullscreen window using the supplied attributes and monitor.
         *
         * @param title
         * @param attributes
         * @param monitor
         * @param mode
         * @return
         */
        static Window *createWindow(GraphicsApi api,
                                    std::string title,
                                    Vec2i size,
                                    WindowAttributes attributes,
                                    Monitor &monitor,
                                    VideoMode mode);
    };
}

#endif //MANA_DISPLAYAPI_HPP
