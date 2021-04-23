#include "render/opengl/oglcheckerror.hpp"

#include "extern/glad.h"

void checkGLError(const std::string &source) {
    GLenum er = glGetError();
    if (er != GL_NO_ERROR) {
        std::string error = source + " GLERROR: ";
        error += std::to_string(er);
        fprintf(stderr, "%s\n", error.c_str());
    }
}