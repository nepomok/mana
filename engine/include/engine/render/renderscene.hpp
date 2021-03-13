#ifndef MANA_RENDERSCENE_HPP
#define MANA_RENDERSCENE_HPP

#include <vector>

#include "engine/render/rendermesh.hpp"
#include "engine/render/renderunit.hpp"
#include "engine/render/renderlight.hpp"
#include "engine/render/rendercamera.hpp"
#include "engine/math/matrix.hpp"

namespace mana {
    /**
     *  A RenderScene object contains the low level rendering scene data, such as camera, mesh and lighting data.
     */
    struct RenderScene {
        RenderCamera camera;

        std::vector<RenderUnit> units;

        std::vector<DirectionalLight> directionalLights;
        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;
    };
}

#endif //MANA_RENDERSCENE_HPP
