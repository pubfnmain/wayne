#include <LScreenshotRequest.h>

#include "Output.h"

Output::Output(const void *params) noexcept : LOutput(params) {}

void Output::paintGL()
{
	LOutput::paintGL();
	for (auto *request : screenshotRequests())
		request->accept(true);
}
