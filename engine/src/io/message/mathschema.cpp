/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "engine/io/schema/mathschema.hpp"

namespace mana {
    Grid &operator<<(Grid &value, const Message &message) {
        value.size << message["size"];
        value.nCol = message["nCol"];
        value.nRow = message["nRow"];
        return value;
    }

    Message &operator<<(Message &message, const Grid &value) {
        auto map = std::map<std::string, Message>();
        map["size"] << value.size;
        map["nCol"] = value.nCol;
        map["nRow"] = value.nRow;
        message = map;
        return message;
    }

    //TODO: Implement matrix schema
    template<typename T, int W, int H>
    Matrix<T, W, H> &operator<<(Matrix<T, W, H> &matrix, const Message &message) {
        throw std::runtime_error("Not implemented");
    }

    template<typename T, int W, int H>
    Message &operator<<(Message &message, const Matrix<T, W, H> &value) {
        throw std::runtime_error("Not implemented");
    }

    template<typename T>
    Rectangle<T> &operator<<(Rectangle<T> &mat, const Message &message) {
        mat.position << message["position"];
        mat.dimensions << message["dimensions"];
        return mat;
    }

    template<typename T>
    Message &operator<<(Message &message, const Rectangle<T> &value) {
        auto map = std::map<std::string, Message>();
        map["position"] << value.position;
        map["dimensions"] << value.dimensions;
        return message;
    }

    Transform &operator<<(Transform &value, const Message &message) {
        value.position << message["position"];
        value.rotation << message["rotation"];
        value.scale << message["scale"];
        return value;
    }

    Message &operator<<(Message &message, const Transform &value) {
        auto map = std::map<std::string, Message>();
        map["position"] << value.position;
        map["rotation"] << value.rotation;
        map["scale"] << value.scale;
        message = map;
        return message;
    }

    template<typename T>
    Vector2<T> &operator<<(Vector2<T> &value, const Message &message) {
        value.x = message["x"];
        value.y = message["y"];
        return value;
    }

    template<typename T>
    Message &operator<<(Message &message, const Vector2<T> &value) {
        auto map = std::map<std::string, Message>();
        map["x"] = value.x;
        map["y"] = value.y;
        message = map;
        return message;
    }

    template<typename T>
    Vector3<T> &operator<<(Vector3<T> &value, const Message &message) {
        value.x = message.value<float>("x", message.value<float>("r", 0));
        value.y = message.value<float>("y", message.value<float>("g", 0));
        value.z = message.value<float>("z", message.value<float>("b", 0));
        return value;
    }

    template<typename T>
    Message &operator<<(Message &message, const Vector3<T> &value) {
        auto map = std::map<std::string, Message>();
        map["x"] = value.x;
        map["y"] = value.y;
        map["z"] = value.z;
        message = map;
        return message;
    }

    template<typename T>
    Vector4<T> &operator<<(Vector4<T> &value, const Message &message) {
        value.x = message.value<float>("x", message.value<float>("r", 0));
        value.y = message.value<float>("y", message.value<float>("g", 0));
        value.z = message.value<float>("z", message.value<float>("b", 0));
        value.w = message.value<float>("w", message.value<float>("a", 0));
        return value;
    }

    template<typename T>
    Message &operator<<(Message &message, const Vector4<T> &value) {
        auto map = std::map<std::string, Message>();
        map["x"] = value.x;
        map["y"] = value.y;
        map["z"] = value.z;
        map["w"] = value.w;
        message = map;
        return message;
    }
}