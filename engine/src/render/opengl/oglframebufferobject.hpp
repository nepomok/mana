#ifndef MANA_OGLFRAMEBUFFEROBJECT_HPP
#define MANA_OGLFRAMEBUFFEROBJECT_HPP

#include "engine/render/framebufferobject.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLFrameBufferObject : public FrameBufferObject {
        public:
            ~OGLFrameBufferObject() override = default;

            virtual Vec2i getSize() const = 0;

            virtual GLuint getFBO() const = 0;
        };
    }
}

#endif //MANA_OGLFRAMEBUFFEROBJECT_HPP
