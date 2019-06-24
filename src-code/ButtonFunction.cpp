#include "Buttons.h"
#include <vector>

namespace XWin
{
	// Button functionality, defined in `ButtonFunction`

	void Buttons::ButtonFunction::A(bool *active)
	{
		keyInput(active, 0x10);
	}

	void Buttons::ButtonFunction::B(bool *active)
	{
		keyInput(active, 0x11);
	}

	void Buttons::ButtonFunction::X(bool *active)
	{
		keyInput(active, 0x12);
	}

	void Buttons::ButtonFunction::Y(bool *active)
	{
		keyInput(active, 0x13);
	}

	void Buttons::ButtonFunction::DPAD_UP(bool *active)
	{
		keyInput(active, 0x02);
	}

	void Buttons::ButtonFunction::DPAD_DOWN(bool *active)
	{
		keyInput(active, 0x04);
	}

	void Buttons::ButtonFunction::DPAD_LEFT(bool *active)
	{
		keyInput(active, 0x05);
	}

	void Buttons::ButtonFunction::DPAD_RIGHT(bool *active)
	{
		keyInput(active, 0x03);
	}

	void Buttons::ButtonFunction::LB(bool *active)
	{
		TRIGGERINVERTED
			? keyInput(active, 0x30)
			: keyInput(active, 0x20);
	}

	void Buttons::ButtonFunction::RB(bool *active)
	{
		if (!TRIGGERINVERTED)
		{
			keyInput(active, 0x21);
			return;
		}

		INPUT mouseInput;
		mouseInput.type = INPUT_MOUSE;

		mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, &mouseInput, sizeof(INPUT));

		mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput(1, &mouseInput, sizeof(INPUT));
	}

	void Buttons::ButtonFunction::START(bool *active)
	{
		keyInput(active, 0x01);
	}

	void Buttons::ButtonFunction::BACK(bool *active)
	{
		keyInput(active, 0x0F);
	}

	void Buttons::ButtonFunction::THUMB_L(bool *active)
	{
		keyInput(active, 0x14);
	}

	void Buttons::ButtonFunction::THUMB_R(bool *active)
	{
		mouseInput(active, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	}

	// Private helper functions

	void Buttons::ButtonFunction::keyInput(bool *active,
		const WORD &scanCode)
	{
		INPUT kbInput;
		kbInput.type = INPUT_KEYBOARD;
		kbInput.ki.wVk = 0;
		kbInput.ki.wScan = scanCode;

		if (*active)
		{
			kbInput.ki.dwFlags = KEYEVENTF_KEYUP;
			*active = false;
		}
		else
		{
			kbInput.ki.dwFlags = KEYEVENTF_SCANCODE;
			*active = true;
		}

		SendInput(1, &kbInput, sizeof(INPUT));
	}

	void Buttons::ButtonFunction::mouseInput(bool *active,
		const WORD &mouseAction1,
		const WORD &mouseAction2)
	{

		INPUT mouseInput;
		mouseInput.type = INPUT_MOUSE;

		if (*active)
		{
			mouseInput.mi.dwFlags = mouseAction2;
			*active = false;
		}
		else
		{
			mouseInput.mi.dwFlags = mouseAction1;
			*active = true;
		}

		SendInput(1, &mouseInput, sizeof(INPUT));
	}
}