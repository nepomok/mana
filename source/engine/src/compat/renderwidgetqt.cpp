#include "compat/renderwidgetqt.hpp"

#include "platform/graphics/opengl-qt/qtoglrenderdevice.hpp"
#include "platform/graphics/opengl-qt/qtoglrendertarget.hpp"

namespace engine {
    RenderWidgetQt::RenderWidgetQt(QWidget *parent, AssetManager &assetManager)
            : QOpenGLWidget(parent), assetManager(assetManager) {}

    std::unique_ptr<RenderTarget> RenderWidgetQt::getWidgetRenderTarget() {
            return std::move(std::make_unique<opengl::QtOGLRenderTarget>(defaultFramebufferObject(),
                                                                         Vec2i(frameSize().rwidth(), frameSize().rheight()),
                                                                         1,
                                                                         false));
    }
}