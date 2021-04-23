#ifndef MANA_PERSPECTIVECAMERA_HPP
#define MANA_PERSPECTIVECAMERA_HPP

#include "engine/render/camera/camera.hpp"

#include "engine/math/matrixmath.hpp"

namespace mana {
    class PerspectiveCamera : public Camera {
    public:
        Mat4f projection() override {
            return MatrixMath::perspective(fov,
                                           aspectRatio,
                                           nearClip,
                                           farClip);
        }

        float fov = 60;
        float aspectRatio = 4.0f / 3.0f;
    };
}

#endif //MANA_PERSPECTIVECAMERA_HPP
