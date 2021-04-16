#ifndef MANA_ORTHOGRAPHICCAMERA_HPP
#define MANA_ORTHOGRAPHICCAMERA_HPP

#include "engine/render/camera.hpp"
#include "engine/math/matrixmath.hpp"

namespace mana {
    class OrthographicCamera : public Camera {
    public:
        Mat4f perspective() override {
            return MatrixMath::ortho(left,
                                     right,
                                     bottom,
                                     top,
                                     nearClip,
                                     farClip);
        }

        float left = 0;
        float top = 0;
        float right = 1;
        float bottom = 1;
    };
}

#endif //MANA_ORTHOGRAPHICCAMERA_HPP
