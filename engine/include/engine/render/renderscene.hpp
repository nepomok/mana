#ifndef MANA_RENDERSCENE_HPP
#define MANA_RENDERSCENE_HPP

#include <vector>

#include "engine/render/camera/camera.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/lighting/light.hpp"

namespace mana {
    struct RenderScene {
        Camera* camera;

        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;

        std::vector<RenderCommand> commands;
    };
}

#endif //MANA_RENDERSCENE_HPP
