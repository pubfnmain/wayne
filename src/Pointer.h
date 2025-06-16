#ifndef POINTER_H
#define POINTER_H

#include <LPointer.h>

using namespace Louvre;

class Pointer final : public LPointer
{
public:
	Pointer(const void *params) noexcept;
};

#endif
