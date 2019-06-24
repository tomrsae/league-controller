#include "Controller.h"

// Initializes static members and defines default values.

namespace XWin
{
	short Controller::TRIGGER_SENSITIVITY = 100;
	float Controller::JOYSTICK_SENSITIVITY = 1;
	int Controller::ATTEMPTED_RECONNECTS = 0;
	bool Buttons::TRIGGERINVERTED = false;

	std::array<Buttons::Button, 14> Buttons::buttons = {
		// Colored ABXY buttons
		Button(ButtonFlag::A,			ButtonFunction::A),
		Button(ButtonFlag::B,			ButtonFunction::B),
		Button(ButtonFlag::X,			ButtonFunction::X),
		Button(ButtonFlag::Y,			ButtonFunction::Y),

		// D-PAD buttons
		Button(ButtonFlag::DPAD_UP,		ButtonFunction::DPAD_UP),
		Button(ButtonFlag::DPAD_DOWN,	ButtonFunction::DPAD_DOWN),
		Button(ButtonFlag::DPAD_LEFT,	ButtonFunction::DPAD_LEFT),
		Button(ButtonFlag::DPAD_RIGHT,	ButtonFunction::DPAD_RIGHT),

		// Shoulder buttons
		Button(ButtonFlag::LB,			ButtonFunction::LB),
		Button(ButtonFlag::RB,			ButtonFunction::RB),

		// Menu buttons
		Button(ButtonFlag::START,		ButtonFunction::START),
		Button(ButtonFlag::BACK,		ButtonFunction::BACK),

		// Thumbstick buttons
		Button(ButtonFlag::THUMB_L,		ButtonFunction::THUMB_L),
		Button(ButtonFlag::THUMB_R,		ButtonFunction::THUMB_R)
	};

	POINT CursorManager::pCursor = POINT();

}