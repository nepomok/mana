#ifndef MANA_OGLMESHOBJECT_HPP
#define MANA_OGLMESHOBJECT_HPP

#include "engine/render/meshobject.hpp"
#include "../../../extern/glad.h"

#include <GLFW/glfw3.h>

namespace mana {
    namespace opengl {
        class OGLMeshObject : public MeshObject {
        public:
            GLuint VAO;
            GLuint VBO;
            GLuint EBO;

            size_t elementCount;

            bool indexed;

            bool instanced;
            size_t instanceCount;
            GLuint instanceVBO;

            GLFWwindow *wndH;

            explicit OGLMeshObject(GLFWwindow *wndH) : VAO(0),
                                                       VBO(0),
                                                       EBO(0),
                                                       elementCount(0),
                                                       indexed(false),
                                                       instanced(false),
                                                       instanceCount(0),
                                                       instanceVBO(0),
                                                       wndH(wndH) {}

            OGLMeshObject(const OGLMeshObject &copy) = delete;

            OGLMeshObject &operator=(const OGLMeshObject &copy) = delete;

            ~OGLMeshObject() override {
                GLFWwindow *originalContext = glfwGetCurrentContext();
                glfwMakeContextCurrent(wndH);
                glDeleteVertexArrays(1, &VAO);
                glDeleteBuffers(1, &VBO);
                if (indexed) {
                    glDeleteBuffers(1, &EBO);
                }
                glDeleteBuffers(1, &instanceVBO);
                if (originalContext != NULL)
                    glfwMakeContextCurrent(originalContext);
            }
        };
    }
}

#endif //MANA_OGLMESHOBJECT_HPP
