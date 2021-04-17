#ifndef MANA_FRAMEBUFFEROBJECT_HPP
#define MANA_FRAMEBUFFEROBJECT_HPP

#include "engine/render/renderobject.hpp"

namespace mana {
    class FrameBufferObject : public RenderObject {
    public:
        ~FrameBufferObject() override = default;
    };
}

#endif //MANA_FRAMEBUFFEROBJECT_HPP
