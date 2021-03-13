#ifndef MANA_FRAMEBUFFEROBJECT_HPP
#define MANA_FRAMEBUFFEROBJECT_HPP

namespace mana {
    class FrameBufferObject {
    protected:
        FrameBufferObject() = default;

        FrameBufferObject(const FrameBufferObject &copy) = default;

        FrameBufferObject &operator=(const FrameBufferObject &) = default;

    public:
        virtual ~FrameBufferObject() = default;
    };
}

#endif //MANA_FRAMEBUFFEROBJECT_HPP
