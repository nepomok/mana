#ifndef MANA_OGLFRAMEBUFFEROBJECT_HPP
#define MANA_OGLFRAMEBUFFEROBJECT_HPP

#include "engine/render/framebufferobject.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLFrameBufferObject : public FrameBufferObject {
        protected:
            OGLFrameBufferObject() = default;

        public:
            OGLFrameBufferObject(const OGLFrameBufferObject &copy) = delete;

            OGLFrameBufferObject &operator=(const OGLFrameBufferObject &copy) = delete;

            ~OGLFrameBufferObject() override = default;

            virtual Vec2i getSize() const = 0;

            virtual GLuint getFBO() const = 0;
        };
    }
}

#endif //MANA_OGLFRAMEBUFFEROBJECT_HPP
