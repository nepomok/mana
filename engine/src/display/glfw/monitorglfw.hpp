#ifndef MANA_MONITORGLFW_HPP
#define MANA_MONITORGLFW_HPP

#include "engine/render/monitor.hpp"

#include "glfwinclude.hpp"

namespace mana {
    class MonitorGLFW : public Monitor {
    public:
        ~MonitorGLFW() override {};

        Vec2i getVirtualPosition() override {
            int posx, posy;
            glfwGetMonitorPos(monH, &posx, &posy);
            return {posx, posy};
        }

        Recti getWorkArea() override {
            int posx, posy, width, height;
            glfwGetMonitorWorkarea(monH, &posx, &posy, &width, &height);
            return {Vec2i(posx, posy), Vec2i(width, height)};
        }

        Vec2i getPhysicalSize() override {
            int sizex, sizey;
            glfwGetMonitorPhysicalSize(monH, &sizex, &sizey);
            return {sizex, sizey};
        }

        Vec2f getContentScale() override {
            float scalex, scaley;
            glfwGetMonitorContentScale(monH, &scalex, &scaley);
            return {scalex, scaley};
        }

        std::string getName() override {
            return std::string(glfwGetMonitorName(monH));
        }

        VideoMode getVideoMode() override {
            const GLFWvidmode *v = glfwGetVideoMode(monH);
            VideoMode ret{};
            ret.width = v->width;
            ret.height = v->height;
            ret.redBits = v->redBits;
            ret.greenBits = v->greenBits;
            ret.blueBits = v->blueBits;
            ret.refreshRate = v->refreshRate;
            return ret;
        }

        std::vector<VideoMode> getSupportedVideoModes() override {
            int count;
            const GLFWvidmode *modes = glfwGetVideoModes(monH, &count);

            std::vector<VideoMode> ret;
            for (int i = 0; i < count; i++) {
                VideoMode r{};
                const GLFWvidmode *v = &modes[i];

                r.width = v->width;
                r.height = v->height;
                r.redBits = v->redBits;
                r.greenBits = v->greenBits;
                r.blueBits = v->blueBits;
                r.refreshRate = v->refreshRate;

                ret.emplace_back(r);
            }

            return ret;
        }

        GLFWmonitor *monH = nullptr;
    };
}

#endif //MANA_MONITORGLFW_HPP
