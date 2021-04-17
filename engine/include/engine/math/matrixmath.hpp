#ifndef MANA_GRAPHICSMATH_HPP
#define MANA_GRAPHICSMATH_HPP

#include "engine/math/vector3.hpp"
#include "engine/math/vector4.hpp"
#include "engine/math/matrix.hpp"

namespace mana {
    namespace MatrixMath {
        Mat4f inverse(const Mat4f &mat);

        Mat4f transpose(const Mat4f &mat);

        /**
         * Returns a matrix which can be used for multiplying with a column vector.
         *
         * The returned matrix should be multiplied with a column vector or transposed to multiply with a row vector.
         *
         * @param fovy
         * @param aspect
         * @param zNear
         * @param zFar
         * @return
         */
        Mat4f perspective(float fovy, float aspect, float zNear, float zFar);

        /**
         * Returns a matrix which can be used for multiplying with a column vector.
         *
         * The returned matrix should be multiplied with a column vector or transposed to multiply with a row vector.
         *
         * @param left
         * @param right
         * @param bottom
         * @param top
         * @param zNear
         * @param zFar
         * @return
         */
        Mat4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);

        /**
         * The returned matrix should be multiplied with a column vector or transposed to multiply with a row vector.
         *
         * @param translationValue
         * @return
         */
        Mat4f translate(const Vec3f &translationValue);

        /**
         * The returned matrix should be multiplied with a column vector or transposed to multiply with a row vector.
         *
         * @param scaleValue
         * @return
         */
        Mat4f scale(const Vec3f &scaleValue);

        /**
         * The returned matrix should be multiplied with a column vector or transposed to multiply with a row vector.
         *
         * @param rotationValue
         * @return
         */
        Mat4f rotate(const Vec3f &rotationValue);
    }
}

#endif //MANA_GRAPHICSMATH_HPP
