#include "engine/compat/qt/renderwidgetqt.hpp"

#include "graphics/opengl-qt/qtoglrenderdevice.hpp"
#include "graphics/opengl-qt/qtoglrendertarget.hpp"

namespace engine {
    RenderWidgetQt::RenderWidgetQt(QWidget *parent, AssetManager &assetManager)
            : QOpenGLWidget(parent), assetManager(assetManager) {}

    std::unique_ptr<RenderTarget> RenderWidgetQt::getWidgetRenderTarget() {
        return std::move(std::make_unique<opengl::QtOGLRenderTarget>(defaultFramebufferObject(),
                                                           Vec2i(frameSize().rwidth(), frameSize().rheight()),
                                                           1,
                                                           false));
    }

    std::unique_ptr<RenderDevice> RenderWidgetQt::getWidgetRenderDevice() {
        return std::move(std::make_unique<opengl::QtOGLRenderDevice>());
    }
}