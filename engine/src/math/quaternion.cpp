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

#include "engine/math/quaternion.hpp"

#include "engine/math/rotation.hpp"
#include "engine/math/matrixmath.hpp"

#include <cmath>
#include <stdexcept>

namespace engine {
    static Vec3f convertToRadian(const Vec3f &degrees) {
        return Vec3f(degreesToRadians(degrees.x), degreesToRadians(degrees.y), degreesToRadians(degrees.z));
    }

    //https://math.stackexchange.com/a/2975462
    static void eulerToQuaternion(Quaternion &q, const Vec3f &eulerDegrees) {
        Vec3f eulerRadians = convertToRadian(eulerDegrees);
        q.x = sinf(eulerRadians.z / 2) * cosf(eulerRadians.y / 2) * cosf(eulerRadians.x / 2) -
              cosf(eulerRadians.z / 2) * sinf(eulerRadians.y / 2) * sinf(eulerRadians.x / 2);

        q.y = cosf(eulerRadians.z / 2) * sinf(eulerRadians.y / 2) * cosf(eulerRadians.x / 2) +
              sinf(eulerRadians.z / 2) * cosf(eulerRadians.y / 2) * sinf(eulerRadians.x / 2);

        q.z = cosf(eulerRadians.z / 2) * cosf(eulerRadians.y / 2) * sinf(eulerRadians.x / 2) -
              sinf(eulerRadians.z / 2) * sinf(eulerRadians.y / 2) * cosf(eulerRadians.x / 2);

        q.w = cosf(eulerRadians.z / 2) * cosf(eulerRadians.y / 2) * cosf(eulerRadians.x / 2) +
              sinf(eulerRadians.z / 2) * sinf(eulerRadians.y / 2) * sinf(eulerRadians.x / 2);
    }

    static Vec3f quaternionToEuler(const Quaternion &q) {
        Vec3f ret;
        float t0 = 2 * (q.w * q.x + q.y * q.z);
        float t1 = 1 - 2 * (q.x * q.x + q.y * q.y);
        ret.z = atan2f(t0, t1);
        float t2 = 2 * (q.w * q.y - q.z * q.x);
        if (t2 > 1)
            t2 = 1;
        else if (t2 < -1)
            t2 = -1;
        ret.y = asinf(t2);
        float t3 = 2 * (q.w * q.z + q.x * q.y);
        float t4 = 1 - 2 * (q.y * q.y + q.z * q.z);
        ret.x = atan2f(t3, t4);
        return ret;
    }

    //https://www.cprogramming.com/tutorial/3d/quaternions.html
    Quaternion::Quaternion()
            : w(1), x(0), y(0), z(0) {}

    Quaternion::Quaternion(float w, float x, float y, float z)
            : w(w), x(x), y(y), z(z) {}

    Quaternion::Quaternion(Vec4f vec)
            : w(vec.w), x(vec.x), y(vec.y), z(vec.z) {}

    Quaternion::Quaternion(Vec3f eulerAngles)
            : w(0), x(0), y(0), z(0) {
        eulerToQuaternion(*this, eulerAngles);
    }

    Quaternion::operator Vec4f() const {
        return Vec4f(x, y, z, w);
    }

    Quaternion Quaternion::operator*(const Quaternion &other) const {
        Quaternion ret;
        ret.w = w * other.w - x * other.x - y * other.y - z * other.z;
        ret.x = w * other.x + x * other.w + y * other.z - z * other.y;
        ret.y = w * other.y - x * other.z + y * other.w + z * other.x;
        ret.z = w * other.z + x * other.y - y * other.x + z * other.w;
        return ret;
    }

    void Quaternion::setEulerAngles(Vec3f eulerAngles) {
        eulerToQuaternion(*this, eulerAngles);
    }

    Vec3f Quaternion::getEulerAngles() const {
        return quaternionToEuler(*this);
    }

    Mat4f Quaternion::matrix() const {
        Mat4f ret = MatrixMath::identity();
        ret.get(0, 0) = powf(w, 2) + powf(x, 2) - powf(y, 2) - powf(z, 2);
        ret.get(1, 0) = 2 * x * y - 2 * w * z;
        ret.get(2, 0) = 2 * x * z + 2 * w * y;
        ret.get(0, 1) = 2 * x * y + 2 * w * z;
        ret.get(1, 1) = powf(w, 2) - powf(x, 2) + powf(y, 2) - powf(z, 2);
        ret.get(2, 1) = 2 * y * z + 2 * w * x;
        ret.get(0, 2) = 2 * x * z - 2 * w * y;
        ret.get(1, 2) = 2 * y * z - 2 * w * x;
        ret.get(2, 2) = powf(w, 2) - powf(x, 2) - powf(y, 2) + powf(z, 2);
        return ret;
    }

    float Quaternion::magnitude() const {
        return sqrtf(powf(w, 2) + powf(x, 2) + powf(y, 2) + powf(z, 2));
    }

    Quaternion Quaternion::normalize(const Quaternion &q) {
        float magnitude = q.magnitude();
        return Quaternion(q.w / magnitude, q.x / magnitude, q.y / magnitude, q.z / magnitude);
    }
}