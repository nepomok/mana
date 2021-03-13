#ifndef MANA_NODE_HPP
#define MANA_NODE_HPP

#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <typeindex>
#include <functional>

#include "component.hpp"

namespace mana {
    /**
     * The basic building block of the scene (Entity).
     * A node contains components.
     *
     * Only a single instance per component type may be attached to a node.
     *
     * Uses c++11 typeindex feature.
     */
    struct Node {
        std::map<std::type_index, Component *> components;

        template<typename T>
        T &getComponent() {
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) == components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " not found");
            return dynamic_cast<T &>(components.at(typeInfo));
        }

        template<typename T>
        void addComponent(const T &component) {
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) != components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " already exists");
            components.at(typeInfo) = dynamic_cast<Component *>(&component);
        }

        template<typename T>
        T *removeComponent() {
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) == components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " not found");
            T *ret = dynamic_cast<T *>(components.at(typeInfo));
            components.erase(typeInfo);
            return ret;
        }

        template<typename T>
        bool hasComponent() {
            const std::type_info &typeInfo = typeid(T);
            return components.find(typeInfo) != components.end();
        }
    };
}
#endif //MANA_NODE_HPP
