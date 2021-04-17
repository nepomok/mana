#ifndef MANA_RENDERCOMMAND_HPP
#define MANA_RENDERCOMMAND_HPP

#include <vector>

#include "engine/render/mesh.hpp"
#include "engine/render/renderunit.hpp"
#include "engine/render/light.hpp"
#include "engine/render/camera.hpp"
#include "engine/math/matrix.hpp"

namespace mana {
    /**
     *  A RenderCommand object describes a render operation in the form of data.
     *
     *  The render api interprets render commands and translates them into the graphics api specific instructions.
     *
     *  The render units are drawn in order.
     */
    struct RenderCommand {
        Camera* camera;

        std::vector<RenderUnit> units;

        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;
    };
}

#endif //MANA_RENDERCOMMAND_HPP
