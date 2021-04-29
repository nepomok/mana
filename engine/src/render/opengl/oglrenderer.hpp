/**
 *  Mana - 3D Engine
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

#ifndef MANA_OGLRENDERER_HPP
#define MANA_OGLRENDERER_HPP

#include "engine/render/renderer.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderer : public Renderer {
        public:
            void setTarget(const RenderTarget &t) override;

            void setScene(const RenderScene &s) override;

            void setViewport(Vec2i offset, Vec2i size) override;

            void setClear(bool cColor, bool cDepth, bool cStencil) override;

            void setClearColor(ColorRGBA cColor) override;

            void setMultiSample(bool sample) override;

            void render() override;

        private:
            const RenderTarget *target = nullptr;
            const RenderScene *scene = nullptr;
            Vec2i viewportOffset = {}, viewportSize = {};
            ColorRGBA clearColorValue = {};
            bool clearColor = true, clearDepth = true, clearStencil = true;
            bool multiSample = false;
        };
    }
}

#endif //MANA_OGLRENDERER_HPP
