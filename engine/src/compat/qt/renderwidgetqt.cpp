#include "engine/compat/qt/renderwidgetqt.hpp"

#include "graphics/opengl-qt/qtoglrenderdevice.hpp"
#include "graphics/opengl-qt/qtoglrendertarget.hpp"

namespace engine {
    RenderWidgetQt::RenderWidgetQt(AssetRenderManager &assetRenderManager) {
        renderDevice = std::make_unique<opengl::QtOGLRenderDevice>();
        ren = std::make_unique<DeferredRenderer>(*renderDevice, assetRenderManager);
    }

    std::unique_ptr<RenderTarget> RenderWidgetQt::getWidgetRenderTarget() {
        return std::make_unique<opengl::QtOGLRenderTarget>(defaultFramebufferObject(),
                                                           Vec2i(frameSize().rwidth(), frameSize().rheight()),
                                                           1);
    }
}