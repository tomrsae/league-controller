#include "Controller.h"
#include <iostream>
#include <string>

#define VER_NUM "BETA 2.5"
#define MAX_RECONNECT_ATTEMPTS 300		// 300 = 5 min; exits program -> Each attempt takes 1 second

void reconnectHandler(bool*);
bool joystickSensitivityPrompt(float*);
bool triggerInvertPrompt();

XWin::Controller* pGamePad = nullptr;

int main()
{
	float joystickSense = 1.0f;
	bool disconnected = true;

	std::cout << "LeagueController " << VER_NUM << "\n\n";

	std::cout << "Please enter the joystick sensitivity you wish to use: 1 is default, 2-3 is optimal (float numbers are allowed, use '.' (Example: 2.5)\n__________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

	if (joystickSensitivityPrompt(&joystickSense))
	{
		std::cout << "\n > Joystick sensitivity was set to " << std::to_string(joystickSense) << '\n';
		XWin::Controller::JOYSTICK_SENSITIVITY = joystickSense;
	}
	else
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "\n [!] Not a number!\n > Joystick sensitivity is set to 1.\n";
		XWin::Controller::JOYSTICK_SENSITIVITY = 1;
	}

	std::cout << "\nInvert triggers/shoulder buttons? (SEE KEYBINDS.TXT)\n * Type out \"yes\" and submit, or tap enter for \"no\"\n";
	std::cout << "___\b\b\b";

	if (triggerInvertPrompt())
	{
		std::cout << "\n > Triggers and LB/RB are now inverted\n";
	}
	else
	{
		std::cout << "\n > Triggers and LB/RB were NOT inverted\n";
	}

	std::cout << "\n\nProgram is now running, and the controller may be used while it is connected\n - To exit, close the program by regular means\n - If the controller disconnects, program execution will wait until it is reconnected (auto-closes after 5 mins)\n\n * This window must be kept open while controller is used.\n\n";

	pGamePad = new XWin::Controller();

	bool attemptConnect = true;

	while (attemptConnect)
	{
		if (pGamePad->isConnected())
		{
			if (disconnected)
			{
				std::cout << " > A connection to the controller was successfully established\n\n";
				disconnected = false;
			}
		}
		else
		{
			if (XWin::Controller::ATTEMPTED_RECONNECTS == 0)
			{
				std::cout << " [!] Controller was disconnected.\n > Attempting to reconnect...\n\n";
				disconnected = true;
			}

			reconnectHandler(&attemptConnect);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	delete pGamePad;

	return 0;
}

void reconnectHandler(bool *attemptConnect)
{
	delete pGamePad;

	pGamePad = new XWin::Controller();

	++XWin::Controller::ATTEMPTED_RECONNECTS;

	if (XWin::Controller::ATTEMPTED_RECONNECTS >= MAX_RECONNECT_ATTEMPTS)
		*attemptConnect = false;
	else
		*attemptConnect = true;
}

bool triggerInvertPrompt()
{
	std::string invertInput;
	bool invert;

	std::cin.ignore(1000, '\n');
	std::getline(std::cin, invertInput);

	if (invertInput == "yes")
	{
		XWin::Buttons::TRIGGERINVERTED = true;
		invert = true;
	}
	else
	{
		XWin::Buttons::TRIGGERINVERTED = false;
		invert = false;
	}

	return invert;
}

bool joystickSensitivityPrompt(float *jsSense)
{
	if (std::cin >> *jsSense)
		return true;
	else
		return false;
}