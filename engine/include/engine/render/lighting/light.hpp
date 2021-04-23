#ifndef MANA_LIGHT_HPP
#define MANA_LIGHT_HPP

#include "engine/math/transform.hpp"

namespace mana {
    struct DirectionalLight {
        Transform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
        Vec3f direction;
    };

    struct PointLight {
        Transform transform;
        Vec3f ambient = Vec3f(0.1f);
        Vec3f diffuse = Vec3f(1.0f);
        Vec3f specular = Vec3f(1.0f);
        float constant = 1;
        float linear = 1;
        float quadratic = 1;
    };

    struct SpotLight {
        Transform transform;
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

#endif //MANA_LIGHT_HPP
