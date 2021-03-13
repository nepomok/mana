#ifndef MANA_GRAPHICSMATH_HPP
#define MANA_GRAPHICSMATH_HPP

#include "engine/math/vector3.hpp"
#include "engine/math/vector4.hpp"
#include "engine/math/matrix.hpp"

namespace mana {
    class MatrixMath {
    public:
        static Mat4f perspective(float fovy, float aspect, float zNear, float zFar);

        static Mat4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);

        static Mat4f identity();

        static Mat4f inverse(const Mat4f &mat);

        static Mat4f translate(const Vec3f &translationValue);

        static Mat4f scale(const Vec3f &scaleValue);

        static Mat4f rotate(const Vec3f &rotationValue);
    };
}

#endif //MANA_GRAPHICSMATH_HPP
