#pragma once
#include <Windows.h>
#include <array>
#include <functional>

#define KEYEVENTF_KEYDOWN 0

namespace XWin
{
	class Buttons									// Defines XBOX 360/One controllers supported buttons and which actions can be performed with them
	{
	public:
		const enum class ButtonFlag					// Flags for the supported XBOX buttons
		{
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000,

			DPAD_UP = 0x0001,
			DPAD_DOWN = 0x0002,
			DPAD_LEFT = 0x0004,
			DPAD_RIGHT = 0x0008,

			LB = 0x0100,
			RB = 0x0200,

			START = 0x0010,
			BACK = 0x0020,

			THUMB_L = 0x0040,
			THUMB_R = 0x0080,

			NONE = 0x0000
		};

		struct Button										// Defines a supported controller button
		{
			ButtonFlag bFlag;								// Button identifier
			std::function<void(bool*)> fBtnFunctionality;	// Button functionality. Supported functionality defined in `ButtonFunctions` struct, and is to be passed with the ctor

			Button(ButtonFlag flag,
				std::function<void(bool*)> func
			)												// Main ctor
			{
				bFlag = flag;
				fBtnFunctionality = func;
			}
		};

		static bool identifyButton(const ButtonFlag &flag)	// Determines whether flag is a supported button and saves it for future reference
		{
			for (auto btn : buttons)
			{
				if (btn.bFlag == flag)
				{
					lastBtnUsed = btn;
					return true;
				}
			}

			return false;
		}

		static Button* getLastClicked() { return &lastBtnUsed; }

		static bool							TRIGGERINVERTED;
		static std::array<Button, 14>		buttons;		// Supported buttons
	private:
		struct ButtonFunction
		{
			// bool parameter determines whether button was released when function is called

			static void A(bool*);	// Mouse left click functionality

			static void B(bool*);	// Imitates BACKSPACE key (for deleting text and going back on history inside browser)

			static void X(bool*);	// Mouse right click functionality

			static void Y(bool*);	// Imitates RETURN(enter) key to make selection in menus and submit forms

			static void DPAD_UP(bool*);	// UP arrow key

			static void DPAD_DOWN(bool*);	// DOWN arrow key 

			static void DPAD_LEFT(bool*);	// LEFT arrow key

			static void DPAD_RIGHT(bool*);	// RIGHT arrow key

			static void LB(bool*);	// notset*

			static void RB(bool*);	// notset*

			static void START(bool*);	// Opens Windows Start menu

			static void BACK(bool*);	// Imitates Windows Key + TAB to select from open processes in current desktop

			static void THUMB_L(bool*);	// Middle mouse button functionality - may be both toggled and held

			static void THUMB_R(bool*);	// notset*

			static void keyInput(bool*,		// Helper function - simulates keypresses on keyboard
				const WORD&);

			static void mouseInput(bool*,		// Helper function - simulates mouse button clicks
				const WORD&,
				const WORD&);
		};
		static Button					lastBtnUsed;		// Stores which button was pressed to perform release action

	};
}