#include <LScreenshotRequest.h>

#include "Output.h"
#include "Global.h"
#include "Compositor.h"

Output::Output(const void *params) noexcept : LOutput(params) {}

void Output::initializeGL()
{
    G::compositor()->scene.handleInitializeGL(this);
}

void Output::paintGL()
{
    G::compositor()->scene.handlePaintGL(this);

	for (auto *request : screenshotRequests())
		request->accept(true);

}

void Output::moveGL()
{
    G::compositor()->scene.handleMoveGL(this);
}

void Output::resizeGL()
{
    G::compositor()->scene.handleResizeGL(this);
}

void Output::uninitializeGL()
{
    G::compositor()->scene.handleUninitializeGL(this);
}

