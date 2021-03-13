#ifndef MANA_RENDERCAMERA_HPP
#define MANA_RENDERCAMERA_HPP

#include "engine/render/rendertransform.hpp"

namespace mana {
    struct RenderCamera {
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

        RenderTransform transform;

        RenderCamera() = default;
    };
}

#endif //MANA_RENDERCAMERA_HPP
