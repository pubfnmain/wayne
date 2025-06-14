#include <LSeat.h>
#include <LKeyboard.h>
#include <LSurface.h>

#include "Toplevel.h"
#include "Global.h"
#include "Compositor.h"

Toplevel::Toplevel(const void *params) : LToplevelRole(params)
{
	Compositor *compositor = G::compositor();
	LSurface *surf = surface();
	LSeat *seat = compositor->seat();
	surf->raise();
	seat->keyboard()->setFocus(surf);
	seat->pointer()->setFocus(surf);

	if (compositor->con) {
		prev = compositor->con;
		next = compositor->con->next;
		next->prev = this;
		prev->next = this;
	} else {
		prev = this; 
		next = this; 
	}
	compositor->con = this;
}

Toplevel::~Toplevel() {
	Compositor *compositor = G::compositor();

	if (prev == this) {
		compositor->con = nullptr;
		return;
	}

	LSeat *seat = compositor->seat();
	LSurface *surface = prev->surface();
	surface->raise();
	seat->keyboard()->setFocus(surface);
	seat->pointer()->setFocus(surface);

	prev->next = next;
	next->prev = prev;

	compositor->con = prev;
}

