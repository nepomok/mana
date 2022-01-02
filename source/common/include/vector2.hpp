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

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>

namespace engine {
    template<typename T>
    class Vector2 {
    public:
        T x;
        T y;

        Vector2()
                : x(0), y(0) {
        }

        explicit Vector2(T v)
                : x(v), y(v) {
        }

        Vector2(T x, T y)
                : x(x), y(y) {
        }

        Vector2<T> &operator+=(const Vector2<T> &v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vector2<T> &operator-=(const Vector2<T> &v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vector2<T> &operator*=(const Vector2<T> &v) {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        Vector2<T> &operator/=(const Vector2<T> &v) {
            x /= v.x;
            y /= v.y;
            return *this;
        }


        Vector2<T> &operator*=(const T &v) {
            x *= v;
            y *= v;
            return *this;
        }

        Vector2<T> &operator/=(const T &v) {
            x /= v;
            y /= v;
            return *this;
        }

        template<typename R>
        Vector2<R> convert() const {
            return Vector2<R>(static_cast<R>(x), static_cast<R>(y));
        }

        friend Vector2<T> operator+(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
        }

        friend Vector2<T> operator-(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
        }

        friend Vector2<T> operator*(const Vector2<T> &lhs, const T &rhs) {
            return Vector2<T>(lhs.x * rhs, lhs.y * rhs);
        }

        friend Vector2<T> operator/(const Vector2<T> &lhs, const T &rhs) {
            return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
        }

        friend Vector2<T> operator*(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
        }

        friend Vector2<T> operator/(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return Vector2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
        }

        friend bool operator==(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        friend bool operator!=(const Vector2<T> &lhs, const Vector2<T> &rhs) {
            return lhs.x != rhs.x && lhs.y != rhs.y;
        }

        //https://en.wikipedia.org/wiki/Euclidean_distance
        static double distance(const Vector2<T> &start, const Vector2<T> end) {
            return std::sqrt(std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2));
        }
    };

    typedef Vector2<bool> Vec2b;
    typedef Vector2<int> Vec2i;
    typedef Vector2<float> Vec2f;
    typedef Vector2<double> Vec2d;
}

#endif //VECTOR2_HPP