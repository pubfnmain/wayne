#include "Surface.h"
#include "Compositor.h"
#include "Global.h"

Surface::Surface(const void *params) :
	LSurface(params),
	view(this, &G::compositor()->surfacesLayer)
{}

LView *Surface::getView() const
{
	return (LView*)&view;
}

void Surface::mappingChanged()
{
	if (cursorRole())
	{
		view.setVisible(false); 
		return;
	}

	/*
	if (mapped()) {
		if (!firstMap) {
			view.setVisible(true); 
			getView()->setVisible(true); 
			getView()->setParent(&G::compositor()->surfacesLayer); 
			raise();
			return;
		}

		firstMap = false;
	}
	*/
}


void Surface::orderChanged()
{
    if (toplevel() && toplevel()->fullscreen())
        return;

    Surface *prevSurface { static_cast<Surface*>(this->prevSurface()) };
    LView *view { getView() };

    while (prevSurface != nullptr)
    {
        if (subsurface() || prevSurface->getView()->parent() == view->parent())
            break;

        prevSurface = static_cast<Surface*>(prevSurface->prevSurface());
    }

    if (prevSurface)
    {
        if (subsurface() || prevSurface->getView()->parent() == getView()->parent())
            view->insertAfter(prevSurface->getView());
    }
    else
        view->insertAfter(nullptr);
}

