#include "Compositor.h"
#include "Seat.h"
#include "Keyboard.h"
#include "Toplevel.h"
#include "Pointer.h"
#include "Output.h"
#include "Surface.h"

/*
void Compositor::initialized()
{
	scene.mainView()->setClearColor({0.f, 0.f, 0.f, 1.f});
}
*/

LFactoryObject *Compositor::createObjectRequest(LFactoryObject::Type objectType, const void *params)
{
	if (objectType == LFactoryObject::Type::LOutput)
		return new Output(params);	

	if (objectType == LFactoryObject::Type::LSurface)
		return new Surface(params);	

	if (objectType == LFactoryObject::Type::LSeat)
		return new Seat(params);	

	if (objectType == LFactoryObject::Type::LPointer)
		return new Pointer(params);	

	if (objectType == LFactoryObject::Type::LKeyboard)
		return new Keyboard(params);

	if (objectType == LFactoryObject::Type::LToplevelRole)
		return new Toplevel(params);

    return nullptr;
}
