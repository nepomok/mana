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

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>

namespace engine {
    template<typename T>
    class Vector3 {
    public:
        T x;
        T y;
        T z;

        Vector3()
                : x(0), y(0), z(0) {
        }

        explicit Vector3(T v)
                : x(v), y(v), z(v) {
        }

        Vector3(T x, T y, T z)
                : x(x), y(y), z(z) {
        }

        Vector3<T> &operator+=(const Vector3<T> &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3<T> &operator-=(const Vector3<T> &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3<T> &operator*=(const Vector3<T> &v) {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        Vector3<T> &operator/=(const Vector3<T> &v) {
            v /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }

        Vector3<T> &operator*=(const T &v) {
            x *= v;
            y *= v;
            z *= v;
            return *this;
        }

        Vector3<T> &operator/=(const T &v) {
            x /= v;
            y /= v;
            z /= v;
            return *this;
        }

        template<typename R>
        Vector3<R> convert() const {
            return Vector3<R>(static_cast<R>(x), static_cast<R>(y), static_cast<R>(z));
        }

        friend Vector3<T> operator+(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        }

        friend Vector3<T> operator-(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        }

        friend Vector3<T> operator*(const Vector3<T> &lhs, const T &rhs) {
            return Vector3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        }

        friend Vector3<T> operator/(const Vector3<T> &lhs, const T &rhs) {
            return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        }

        friend Vector3<T> operator*(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return Vector3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        }

        friend Vector3<T> operator/(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return Vector3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        }

        friend bool operator==(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        friend bool operator!=(const Vector3<T> &lhs, const Vector3<T> &rhs) {
            return lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z;
        }

        //https://en.wikipedia.org/wiki/Euclidean_distance
        static double distance(const Vector3<T> &start, const Vector3<T> end) {
            return std::sqrt(
                    std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2) + std::pow(start.z - end.z, 2));
        }
    };

    typedef Vector3<bool> Vec3b;
    typedef Vector3<int> Vec3i;
    typedef Vector3<float> Vec3f;
    typedef Vector3<double> Vec3d;
}

#endif //VECTOR3_HPP