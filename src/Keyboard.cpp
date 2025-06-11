#include <LKeyboardKeyEvent.h>
#include <LLauncher.h>

#include "Keyboard.h"
#include "Global.h"
#include "Compositor.h"

Keyboard::Keyboard(const void *params) noexcept : LKeyboard(params)
{
    setKeymap(
        nullptr,
        nullptr,
        "us,ru",
        nullptr,
        "grp:alt_shift_toggle,ctrl:nocaps"
    );
}

void Keyboard::keyEvent(const LKeyboardKeyEvent &event)
{
	const bool LEFT_META  { isKeyCodePressed(KEY_LEFTMETA)  };
    const bool LEFT_SHIFT { isKeyCodePressed(KEY_LEFTSHIFT) };
    const bool LEFT_ALT   { isKeyCodePressed(KEY_LEFTALT)   };
    const bool LEFT_CTRL  { isKeyCodePressed(KEY_LEFTCTRL)  };

	const UInt8 keyCode = event.keyCode();
    if (event.state() == LKeyboardKeyEvent::Pressed) {
		if (LEFT_META && keyCode == KEY_ESC)
			compositor()->finish();
		if (LEFT_META && keyCode == KEY_P)
			LLauncher::launch("wmenu-run -f 'JetBrains Mono 12'");
		if (LEFT_META && LEFT_SHIFT && keyCode == KEY_ENTER)
			LLauncher::launch("alacritty");
		// if (LEFT_ALT && keyCode == KEY_F4)
    }

	G::compositor()->scene.handleKeyboardKeyEvent(event);
}
