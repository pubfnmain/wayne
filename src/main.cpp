#include <unistd.h>

#include <LLauncher.h>
#include <LLog.h>

#include "Compositor.h"

using namespace Louvre;

int main(int, char *[])
{
	setenv("LOUVRE_DEBUG", "3", 0);
	setenv("SRM_DEBUG", "1", 0);
	setenv("LOUVRE_WAYLAND_DISPLAY", "wayland-2", 0);

	LLauncher::startDaemon();

	Compositor compositor;

	LLauncher::launch("~/.config/wayne/autostart");

	if (!compositor.start())
	{
		LLog::fatal("[wayne] Failed to start compositor.");
		return 1;
	}

	while (compositor.state() != LCompositor::Uninitialized)
		compositor.processLoop(-1);

	return 0;
}
