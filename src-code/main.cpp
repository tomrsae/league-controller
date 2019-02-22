#include "Controller.h"
#include <iostream>
#include <string>

#define VER_NUM "BETA 2.4.4"
#define MAX_RECONNECT_ATTEMPTS 300		// 300 = 5 min; exits program -> Each attempt takes 1 second

bool reconnectHandler();
bool triggerInvert();

static bool disconnected = true;
XWin::Controller* gPad;

int main()
{
	std::cout << "LeagueController " << VER_NUM << "\n\n";

	std::cout << "Please enter the joystick sensitivity you wish to use: 1 is default, 2-3 is optimal (float numbers are allowed, use '.' (Example: 2.5)\n__________________________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
	
	if (!(std::cin >> XWin::Controller::JOYSTICK_SENSITIVITY))
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Not a number!\nSensitivity is set to 1.\n";
		XWin::Controller::JOYSTICK_SENSITIVITY = 1;
	}

	std::cout << "\nInvert triggers/shoulder buttons? (SEE KEYBINDS.TXT)\n * Type out \"yes\" and submit, or tap enter for \"no\"\n";
	std::cout << "___\b\b\b";
	if (triggerInvert())
	{
		std::cout << "\n > Triggers and LB/RB are now inverted\n";
	}
	else
	{
		std::cout << "\n > Triggers and LB/RB were NOT inverted\n";
	}

	std::cout << "\nProgram is now running, and the controller may be used while it is connected\n - To exit, close the program by regular means\n - If controller disconnects, program execution will wait until it is reconnected (auto-closes after 5 mins)\n\n * This window must be kept open while controller is used.\n\n";
	
	gPad = new XWin::Controller();

	bool attemptConnect = true;

	while (attemptConnect)
	{
		if (gPad->isConnected())
		{
			if (disconnected)
			{
				std::cout << "A connection to the controller was successfully established\n\n";
				disconnected = false;
			}
		}
		else
		{
			attemptConnect = reconnectHandler();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	delete gPad;

	return 0;
}

// Fix reconnection
bool reconnectHandler()
{
	if (XWin::Controller::ATTEMPTED_RECONNECTS == 0)
	{
		std::cout << "Controller was disconnected.\nAttempting to reconnect...\n\n";
		disconnected = true;
	}

	delete gPad;

	gPad = new XWin::Controller();

	++XWin::Controller::ATTEMPTED_RECONNECTS;

	if (XWin::Controller::ATTEMPTED_RECONNECTS > MAX_RECONNECT_ATTEMPTS)
		return false;
	else
		return true;
}

bool triggerInvert()
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