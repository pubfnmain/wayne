#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <LToplevelRole.h>

using namespace Louvre;

class Toplevel final : public LToplevelRole
{
public:
	Toplevel(const void *params);
	~Toplevel();
	Toplevel *prev; //= nullptr;
	Toplevel *next; //= nullptr;
	void configureRequest();
	// void atomsChanged(LBitset<AtomChanges>, const Atoms &prev) override;
};

#endif
