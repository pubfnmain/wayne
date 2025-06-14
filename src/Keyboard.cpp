#include <LKeyboardKeyEvent.h>
#include <LLauncher.h>

#include "Keyboard.h"
#include "Global.h"
#include "Compositor.h"
#include "Toplevel.h"

void cmdFocus(bool next) {
	Compositor *compositor = G::compositor();
	LKeyboard *keyboard = compositor->seat()->keyboard();
	LPointer *pointer= compositor->seat()->pointer();

	if (keyboard->grab())
		return;

	Toplevel *source = compositor->con;
	if (!source)
		return;

	Toplevel *target;
	if (keyboard->focus()) {
		if (next) {
			target = source->next;
		} else {
			target = source->prev;
		}
	} else {
		target = source;
	}

	LSurface *surface = target->surface();
	surface->raise();
	keyboard->setFocus(surface);
	pointer->setFocus(surface);
}

Keyboard::Keyboard(const void *params) noexcept : LKeyboard(params)
{
	setRepeatInfo(32, 500);
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
	if (event.state() == LKeyboardKeyEvent::Pressed) {
		const int  mod = (isKeyCodePressed(KEY_LEFTMETA) << 3)
			| (isKeyCodePressed(KEY_LEFTSHIFT) << 2)
			| (isKeyCodePressed(KEY_LEFTALT) << 1)
			| isKeyCodePressed(KEY_LEFTCTRL);
		const UInt8 key = event.keyCode();

		switch (mod) {
			case 0b0000:
				if (key == KEY_F11) {
					return;
				}
				break;
			case 0b1100:
				if (key == KEY_ENTER) {
					LLauncher::launch("alacritty");
					return;
				}
				break;
			case 0b1000:
				if (key == KEY_ESC) {
					G::compositor()->finish();
					return;
				} else if (key == KEY_SPACE) {
					LLauncher::launch("wmenu-run -f 'JetBrains Mono 12'");
					return;
				}
				break;
			case 0b0010:
				switch (key) {
					case KEY_TAB:
						cmdFocus(true);
						return;
					case KEY_F4:
						LSurface *surface = focus();
						if (surface) {
							Toplevel *toplevel = (Toplevel*)surface->toplevel();
							if (toplevel) {
								toplevel->close();
								return;
							}
						}
						break;
				}
				break;
		}
	}
	G::compositor()->scene.handleKeyboardKeyEvent(event);
}
