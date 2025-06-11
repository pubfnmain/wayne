#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <LCompositor.h>
#include <LScene.h>

using namespace Louvre;

class Compositor final : public LCompositor
{
public:
	LScene scene;
	LFactoryObject *createObjectRequest(LFactoryObject::Type objectType, const void *params) override;
};

#endif
