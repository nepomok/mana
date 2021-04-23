#ifndef MANA_OGLMESHOBJECT_HPP
#define MANA_OGLMESHOBJECT_HPP

#include "engine/render/rendermesh.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLMeshObject : public RenderMesh {
        public:
            GLuint VAO;
            GLuint VBO;
            GLuint EBO;

            size_t elementCount;

            bool indexed;

            bool instanced;
            size_t instanceCount;
            GLuint instanceVBO;

            explicit OGLMeshObject() : VAO(0),
                                       VBO(0),
                                       EBO(0),
                                       elementCount(0),
                                       indexed(false),
                                       instanced(false),
                                       instanceCount(0),
                                       instanceVBO(0) {}

            OGLMeshObject(const OGLMeshObject &copy) = delete;

            OGLMeshObject &operator=(const OGLMeshObject &copy) = delete;

            ~OGLMeshObject() override {
                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                if (indexed) {
                    glDeleteBuffers(1, &EBO);
                }
                glDeleteBuffers(1, &instanceVBO);
            }
        };
    }
}

#endif //MANA_OGLMESHOBJECT_HPP
