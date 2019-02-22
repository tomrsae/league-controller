#include "Controller.h"

namespace XWin
{
	Controller::Controller()
	{
		m_mainThread = std::this_thread::get_id();
		m_buttonActive = false;
		m_initConnectionFinished = false;
	}

	Controller::Controller(const Controller &)
	{
	}

	Controller::~Controller()
	{
		if (std::this_thread::get_id() == m_mainThread)
		{
			std::thread::id a = std::this_thread::get_id();
			if (m_tAsyncJoystickThread.joinable())
				m_tAsyncJoystickThread.join();

			if (m_tAsyncTriggerThread.joinable())
				m_tAsyncTriggerThread.join();

			if (m_tAsyncButtonThread.joinable())
				m_tAsyncButtonThread.join();

		}
	}

	bool Controller::isConnected()
	{
		ZeroMemory(&m_xsState, sizeof(XINPUT_STATE));

		if (XInputGetState(0, &m_xsState) == ERROR_SUCCESS)
		{
			if (!m_initConnectionFinished
				&& std::this_thread::get_id() == m_mainThread)
			{
				m_tAsyncJoystickThread	= std::thread(&Controller::registerJoystickInput, this);
				m_tAsyncTriggerThread	= std::thread(&Controller::registerTriggerInput, this);
				m_tAsyncButtonThread	= std::thread(&Controller::registerButtonInput, this);

				m_initConnectionFinished = true;
			}

			if (ATTEMPTED_RECONNECTS != 0)
				ATTEMPTED_RECONNECTS = 0;

			return true;
		}
		else
		{
			if (std::this_thread::get_id() == m_mainThread)
			{
				if (m_tAsyncJoystickThread.joinable())
					m_tAsyncJoystickThread.join();

				if (m_tAsyncTriggerThread.joinable())
					m_tAsyncTriggerThread.join();

				if (m_tAsyncButtonThread.joinable())
					m_tAsyncButtonThread.join();
			}

			return false;
		}
	}

	void Controller::registerJoystickInput()
	{
		while (isConnected())
		{
			if (!m_joysticks.L_inDeadZone())
			{
				CursorManager::Move(
					static_cast<short>(m_joysticks.L_getPos_x() * JOYSTICK_SENSITIVITY),
					static_cast<short>(m_joysticks.L_getPos_y() * JOYSTICK_SENSITIVITY)
				);
			}

			if (!m_joysticks.R_inDeadZone())
			{
				// Implement camera movement later
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(7));
		}
	}

	void Controller::registerTriggerInput()
	{
		while (isConnected())
		{
			if (!m_triggers.L_inDeadZone())
			{
				INPUT kbInput;
				kbInput.type = INPUT_KEYBOARD;
				kbInput.ki.wVk = 0;

				Buttons::TRIGGERINVERTED
					? kbInput.ki.wScan = 0x20
					: kbInput.ki.wScan = 0x30;

				kbInput.ki.dwFlags = KEYEVENTF_SCANCODE;
				SendInput(1, &kbInput, sizeof(INPUT));

				kbInput.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &kbInput, sizeof(INPUT));
			}

			if (!m_triggers.R_inDeadZone())
			{
				if (Buttons::TRIGGERINVERTED)
				{
					INPUT kbInput;
					kbInput.type = INPUT_KEYBOARD;
					kbInput.ki.wVk = 0;

					kbInput.ki.wScan = 0x21;

					kbInput.ki.dwFlags = KEYEVENTF_SCANCODE;
					SendInput(1, &kbInput, sizeof(INPUT));

					kbInput.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &kbInput, sizeof(INPUT));
				}
				else
				{
					INPUT mouseInput;
					mouseInput.type = INPUT_MOUSE;

					mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
					SendInput(1, &mouseInput, sizeof(INPUT));

					mouseInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
					SendInput(1, &mouseInput, sizeof(INPUT));
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(TRIGGER_SENSITIVITY));
		}
	}

	void Controller::registerButtonInput()
	{
		while (isConnected())
		{
			if (m_buttonActive || static_cast<Buttons::ButtonFlag>(m_xsState.Gamepad.wButtons) != Buttons::ButtonFlag::NONE)
			{
				Buttons::ButtonFlag activeButton = static_cast<Buttons::ButtonFlag>(m_xsState.Gamepad.wButtons);

				if (m_buttonActive
					&& activeButton != Buttons::ButtonFlag::NONE)
				{
					// Button is being held down
				}

				if (!m_buttonActive
					&& activeButton != Buttons::ButtonFlag::NONE)
				{
					// Button is clicked
					if (Buttons::identifyButton(activeButton))
					{
						Buttons::getLastClicked()->fBtnFunctionality(&m_buttonActive);
					}
				}

				if (m_buttonActive
					&& activeButton == Buttons::ButtonFlag::NONE)
				{
					// Button is released
					Buttons::getLastClicked()->fBtnFunctionality(&m_buttonActive);
				}
			}

			std::this_thread::sleep_for(std::chrono::microseconds(25));
		}
	}
}

