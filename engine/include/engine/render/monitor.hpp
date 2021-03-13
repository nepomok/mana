#ifndef MANA_MONITOR_HPP
#define MANA_MONITOR_HPP

#include <vector>
#include <string>

#include "engine/render/videomode.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/rectangle.hpp"

namespace mana {
    class Monitor {
    public:
        virtual ~Monitor() = default;

        virtual Vec2i getVirtualPosition() = 0;

        virtual Recti getWorkArea() = 0;

        virtual Vec2i getPhysicalSize() = 0;

        virtual Vec2f getContentScale() = 0;

        virtual std::string getName() = 0;

        virtual VideoMode getVideoMode() = 0;

        virtual std::vector<VideoMode> getSupportedVideoModes() = 0;
    };
}

#endif //MANA_MONITOR_HPP
