#ifndef MANA_RENDERLIGHT_HPP
#define MANA_RENDERLIGHT_HPP

#include "engine/render/rendertransform.hpp"

namespace mana {
    /**
     * The graphics api independent light definition.
     *
     * The data is converted into some graphics api dependent shader input constants by the graphics api implementation.
     */
    struct DirectionalLight {
        RenderTransform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
        Vec3f direction;
    };

    struct PointLight {
        RenderTransform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
        float constant = 1;
        float linear = 1;
        float quadratic = 1;
    };

    struct SpotLight {
        RenderTransform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
        Vec3f direction = Vec3f(0, -1, 0);
        float cutOff = 10;
        float outerCutOff = 14;
        float constant = 0.1;
        float linear = 0.1;
        float quadratic = 0.1;
    };
}

#endif //MANA_RENDERLIGHT_HPP
