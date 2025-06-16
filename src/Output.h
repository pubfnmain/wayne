#include <LOutput.h>

using namespace Louvre;

class Output final : public LOutput
{
public:
	Output(const void *params) noexcept;
	void initializeGL() override;
	void resizeGL() override;
	void moveGL() override;
	void paintGL() override;
	void uninitializeGL() override;
};
