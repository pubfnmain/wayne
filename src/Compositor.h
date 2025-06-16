#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <LCompositor.h>
#include <LScene.h>
#include <LSceneView.h>

#include <LLayerView.h>

#include "Toplevel.h"

using namespace Louvre;

class Compositor final : public LCompositor
{
public:
	// void initialized() override;
	LScene scene {};
    LLayerView rootView { scene.mainView() };
    LLayerView backgroundLayer { &rootView };
    LLayerView surfacesLayer { &rootView };
    LLayerView workspacesLayer { &rootView };
    LLayerView fullscreenLayer { &rootView };
    LLayerView overlayLayer { &rootView };
    LLayerView tooltipsLayer { &rootView };
    LLayerView cursorLayer { &rootView };
	LFactoryObject *createObjectRequest(LFactoryObject::Type objectType, const void *params) override;
	Toplevel *con = nullptr;
};

#endif
