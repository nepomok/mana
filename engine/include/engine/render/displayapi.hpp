#ifndef MANA_DISPLAYAPI_HPP
#define MANA_DISPLAYAPI_HPP

#include <functional>
#include <set>

#include "engine/render/window.hpp"
#include "engine/render/windowattributes.hpp"
#include "engine/render/graphicsapi.hpp"

namespace mana {
    /**
     * The polymorphic abstraction around a render api such as glfw or a custom implementation.
     *
     * Display api is responsible for instantiating windows for a specific graphics api
     * and retrieving graphics api independent render related information such as monitors.
     */
    class DisplayAPI {
    public:
        virtual ~DisplayAPI() = default;

        virtual Monitor *getPrimaryMonitor() = 0;

        virtual std::set<Monitor *> getMonitors() = 0;

        /**
         * Create a window with default attributes.
         *
         * @return
         */
        virtual Window *createWindow(GraphicsApi api) = 0;

        /**
         * Create a window using the supplied attributes.
         *
         * @param title
         * @param size
         * @param attributes
         * @return
         */
        virtual Window *createWindow(GraphicsApi api, std::string title, Vec2i size, WindowAttributes attributes) = 0;

        /**
         * Create a fullscreen window using the supplied attributes and monitor.
         *
         * @param title
         * @param attributes
         * @param monitor
         * @param mode
         * @return
         */
        virtual Window *createWindow(GraphicsApi api,
                                     std::string title,
                                     Vec2i size,
                                     WindowAttributes attributes,
                                     Monitor &monitor,
                                     VideoMode mode) = 0;
    };
}

#endif //MANA_DISPLAYAPI_HPP
