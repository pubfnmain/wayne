#ifndef SEAT_H
#define SEAT_H

#include <LSeat.h>

using namespace Louvre;

class Seat final : public LSeat
{
public:
	Seat(const void *params);

    void enabledChanged() override;
    void inputDevicePlugged(LInputDevice *device) override;
    void nativeInputEvent(void *event) override;
    void configureInputDevices() noexcept;
};

#endif
