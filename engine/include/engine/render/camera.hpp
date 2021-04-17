#ifndef MANA_CAMERA_HPP
#define MANA_CAMERA_HPP

#include "engine/render/transform.hpp"
#include "engine/math/matrix.hpp"
#include "engine/math/matrixmath.hpp"

namespace mana {
    class Camera {
    public:
        virtual Mat4f view() const {
            Mat4f ret = MatrixMath::rotate(transform.rotation);
            //The engines move the universe (Negate camera position)
            return ret * MatrixMath::translate(transform.position * -1);
        }

        virtual Mat4f perspective() = 0;

        Transform transform;

        float nearClip = 0.1f;
        float farClip = 1000.0f;
    };
}

#endif //MANA_CAMERA_HPP
