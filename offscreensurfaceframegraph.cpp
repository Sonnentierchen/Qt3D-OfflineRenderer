#include "offscreensurfaceframegraph.h"

OffscreenSurfaceFrameGraph::OffscreenSurfaceFrameGraph(Qt3DCore::QNode* parent, Qt3DRender::QCamera *camera, const QSize &size) :
    Qt3DRender::QRenderSurfaceSelector(parent),
    camera(camera)
{
    offscreenSurface = new QOffscreenSurface();
    QSurfaceFormat format;
    format.setDepthBufferSize(32);
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);
    offscreenSurface->setFormat(format);
    offscreenSurface->create();
    setSurface(offscreenSurface);
    setExternalRenderTargetSize(size);

    // Direct rendering into a texture
    renderTargetSelector = new Qt3DRender::QRenderTargetSelector(this);
    textureTarget = new TextureRenderTarget(renderTargetSelector, size);
    renderTargetSelector->setTarget(textureTarget);

    clearBuffers = new Qt3DRender::QClearBuffers(renderTargetSelector);
    // Does not matter, as we draw the image over the whole background anyway
    clearBuffers->setClearColor(QColor(100, 100, 100, 255));
    clearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);

    viewport = new Qt3DRender::QViewport(renderTargetSelector);
    viewport->setNormalizedRect(QRectF(0.0, 0.0, 1.0, 1.0));

    cameraSelector = new Qt3DRender::QCameraSelector(viewport);
    cameraSelector->setCamera(camera);
}

void OffscreenSurfaceFrameGraph::setSize(const QSize &size) {
    textureTarget->setSize(size);
    setExternalRenderTargetSize(size);
}

Qt3DCore::QNode *OffscreenSurfaceFrameGraph::getLastNode() {
    return cameraSelector;
}
