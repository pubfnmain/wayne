#include "Compositor.h"
#include "Seat.h"
#include "Keyboard.h"

LFactoryObject *Compositor::createObjectRequest(LFactoryObject::Type objectType, const void *params)
{
	if (objectType == LFactoryObject::Type::LSeat)
		return new Seat(params);
	
	if (objectType == LFactoryObject::Type::LKeyboard)
		return new Keyboard(params);

    return nullptr;
}
