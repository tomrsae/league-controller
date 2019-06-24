#pragma once
#include <thread>
#include "Buttons.h"
#include "Joysticks.h"
#include "Triggers.h"
#include "CursorManager.h"

namespace XWin {
	class Controller
	{
	private:
		// Utility
		std::thread::id		m_mainThread;			// ID of main thread, used to make sure threads are joined to the proper main thread, not to itself/eachother
		bool			m_initConnectionFinished;	// Becomes true once first connection was successful and threads are initialized. False by default
		XINPUT_STATE		m_xsState;			// Contains information about the current connected controller
		std::thread		m_tAsyncJoystickThread;		// Thread responsible for calling `registerJoystickInput` asynchronously
		std::thread		m_tAsyncTriggerThread;		// Thread responsible for calling `registerTriggerInput` asynchronously
		std::thread		m_tAsyncButtonThread;		// Thread responsible for calling `registerButtonInput` asynchronously

		// Functionality
		Joysticks		m_joysticks			// Joystick information
						= Joysticks(&m_xsState.Gamepad);
		Triggers		m_triggers			// Trigger information
						= Triggers(&m_xsState.Gamepad);

		void			registerJoystickInput();	// Loop that checks if joystick(s) is outside of deadzone. Perform action if joystick(s) is outside of deadzone
		void			registerTriggerInput();		// Loop that checks if trigger(s) is outside of deadzone. Perform action if trigger(s) is outside of deadzone
		void			registerButtonInput();		// Loop that checks if button has been clicked/released. Perform corresponding buitton action if button was clicked/released
	public:
		Controller();						// Default ctor, initializes members
		Controller(const Controller &);				// Copy ctor, does nothing - Not supposed to be called
		~Controller();						// Destructor, joins threads if called from `m_mainThread`

		static short	TRIGGER_SENSITIVITY;			// How sensititve the triggers are -> 100 by default
		static float	JOYSTICK_SENSITIVITY;			// How sensitive the joysticks are -> 1 by default
		static int		ATTEMPTED_RECONNECTS;		// How many reconnect attempts have been made. 0 by default and is incremented if connection is disconnected. Reset if connection is successful

		bool isConnected();					// Checks if a controller is connected
	};
}

