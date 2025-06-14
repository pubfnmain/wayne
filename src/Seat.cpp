#include <LCompositor.h>
#include <LInputDevice.h>
#include <libinput.h>
#include "Seat.h"

Seat::Seat(const void *params) : LSeat(params) {}

void Seat::enabledChanged()
{
	configureInputDevices();
}

void Seat::inputDevicePlugged(LInputDevice *device)
{
    L_UNUSED(device)
    configureInputDevices();
}

void Seat::nativeInputEvent(void *event)
{
    if (compositor()->inputBackendId() == LInputBackendLibinput)
    {
        libinput_event *ev { static_cast<libinput_event*>(event) };
        L_UNUSED(ev);
    }
}

void Seat::configureInputDevices() noexcept
{
    if (compositor()->inputBackendId() != LInputBackendLibinput)
        return;

    libinput_device *dev;

    for (LInputDevice *device : inputDevices())
    {
        if (!device->nativeHandle())
            continue;

        dev = static_cast<libinput_device*>(device->nativeHandle());

        // libinput_device_config_dwt_set_enabled(dev, LIBINPUT_CONFIG_DWT_ENABLED);
        libinput_device_config_tap_set_enabled(dev, LIBINPUT_CONFIG_TAP_ENABLED);
        libinput_device_config_tap_set_drag_enabled(dev, LIBINPUT_CONFIG_DRAG_ENABLED);;
        libinput_device_config_scroll_set_natural_scroll_enabled(dev, 1);
    }
}
