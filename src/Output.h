#include <LOutput.h>

using namespace Louvre;

class Output final : public LOutput
{
public:
	Output(const void *params) noexcept;
	void paintGL() override;
};
