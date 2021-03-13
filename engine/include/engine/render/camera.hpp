#ifndef MANA_CAMERA_HPP
#define MANA_CAMERA_HPP

#include "engine/render/transform.hpp"

namespace mana {
    //TODO: Refactor camera definition
    struct Camera {
        struct PerspectiveData {
            float fov = 60;
            float aspectRatio = 4.0f / 3.0f;
            float nearClip = 0.1f;
            float farClip = 100.0f;
        };

        struct OrthographicData {
            float left;
            float right;
            float top;
            float bottom;
            float nearClip = 0.1f;
            float farClip = 1000.0f;
        };

        enum Type {
            PERSPECTIVE,
            ORTHOGRAPHIC
        } cameraType = PERSPECTIVE;

        union Data {
            PerspectiveData perspective;
            OrthographicData orthographic;

            Data() : perspective(PerspectiveData()) {
            }
        } cameraData;

        Transform transform;

        Camera() = default;
    };
}

#endif //MANA_CAMERA_HPP
