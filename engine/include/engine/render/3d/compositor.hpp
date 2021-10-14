/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_COMPOSITOR_HPP
#define MANA_COMPOSITOR_HPP

#include "engine/render/3d/geometrybuffer.hpp"

#include "engine/render/rendercommand.hpp"

namespace engine {
    /**
     * The compositor creates the final image on the screen by combining textures from the geometry buffer.
     */
    class Compositor {
    public:
        struct Layer {
            // The names of the color textures in the geometry buffer
            std::vector<std::string> color;

            //The optional name of the depth texture in the geometry buffer
            std::string depth;

            //The depth test mode to use when rendering this layer
            DepthTestMode depthTestMode = DEPTH_TEST_LESS;

            //The blend modes to use when rendering this layer
            BlendMode colorBlendModeSource = SRC_ALPHA;
            BlendMode colorBlendModeDest = ONE_MINUS_SRC_ALPHA;
        };

        Compositor() = default;

        explicit Compositor(RenderDevice *device);

        Compositor(RenderDevice *device, std::vector<Layer> layers);

        std::vector<Layer> &getLayers();

        void setClearColor(ColorRGB color);

        void presentLayers(RenderTarget &screen, GeometryBuffer &buffer);

        void presentLayers(RenderTarget &screen, GeometryBuffer &buffer, const std::vector<Layer> &pLayers);

    private:
        void drawLayer(RenderTarget &screen,
                       GeometryBuffer &buffer,
                       const Layer &layer);

        ColorRGB clearColor{0, 0, 0};
        RenderDevice *device{};
        std::vector<Layer> layers;
        std::unique_ptr<ShaderProgram> shader;
    };
}

#endif //MANA_COMPOSITOR_HPP
