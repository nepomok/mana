#ifndef MANA_GLFWINPUT_HPP
#define MANA_GLFWINPUT_HPP

#include <set>
#include <map>

#include <GLFW/glfw3.h>

#include "engine/input/input.hpp"

namespace mana {
    class GLFWInput : public Input {
    public:
        explicit GLFWInput(GLFWwindow &wndH);

        ~GLFWInput() override;

        void glfwKeyCallback(int key, int scancode, int action, int mods);

        void glfwCursorCallback(double xpos, double ypos);

        void glfwMouseKeyCallback(int button, int action, int mods);

        bool getKeyDown(Key key) override;

        Mouse getMouse() override;

        void registerListener(InputListener &listener) override;

        void unregisterListener(InputListener &listener) override;

        void setClipboardText(std::string text) override;

        std::string getClipboardText() override;

    private:
        GLFWwindow &wndH;

        std::set<InputListener *> listeners;

        Mouse mouse;
        std::map<Key, bool> keys;
    };
}

#endif //MANA_GLFWINPUT_HPP
