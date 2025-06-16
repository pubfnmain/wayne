#include <LSurface.h>
#include <LSurfaceView.h>

using namespace Louvre;

class Surface final : public LSurface
{
public:
	Surface(const void *params);

	LSurfaceView view;

	LView *getView() const;

	void mappingChanged();
	void orderChanged();

	// bool firstMap = true;
};
