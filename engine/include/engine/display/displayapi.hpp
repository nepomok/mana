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
