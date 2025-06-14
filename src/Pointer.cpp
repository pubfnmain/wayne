#include "Pointer.h"
#include "Global.h"
#include "Compositor.h"

Pointer::Pointer(const void *params) noexcept : LPointer(params) {};

void Pointer::focusChanged()
{
	LSurface *surface = focus();
	if (!surface)
		return;
	Toplevel *target = (Toplevel*)surface->toplevel();
	if (target)
		G::compositor()->con = target;
}
