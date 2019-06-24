#pragma once
#include <Windows.h>
#include <array>
#include <functional>

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
		public:
			ButtonFlag bFlag;								// Button identifier
			bool active;

			void performAction()
			{
				fBtnFunctionality(&active);
			}

			Button(ButtonFlag flag, std::function<void(bool*)> func)
				: bFlag(flag), fBtnFunctionality(func), active(false) { }

			Button() : bFlag(ButtonFlag::NONE), fBtnFunctionality([](bool) { return false; }), active(false) { }
		private:
			std::function<void(bool*)> fBtnFunctionality;	// Button functionality. Supported functionality defined in `ButtonFunctions` struct, and is to be passed with the ctor
		};

		static bool identifyButton(const ButtonFlag &flag, Button *identifiedButton)	// Determines whether flag is a supported button and saves it for future reference
		{
			for (auto btn : buttons)
			{
				if (btn.bFlag == flag)
				{
					if (!identifiedButton->active)
						*identifiedButton = btn;
					return true;
				}
			}

			identifiedButton = nullptr;
			return false;
		}

		static bool							TRIGGERINVERTED;	// Determines whether triggers should be inverted
		static std::array<Button, 14>		buttons;			// Supported buttons
	private:
		struct ButtonFunction
		{
			// bool parameter determines whether button is pressed or released when function is called

			static void A(bool*);			// Q Spell

			static void B(bool*);			// W Spell

			static void X(bool*);			// E Spell

			static void Y(bool*);			// R Spell

			static void DPAD_UP(bool*);		// Active item 1

			static void DPAD_RIGHT(bool*);	// Active item 2

			static void DPAD_DOWN(bool*);	// Trinket

			static void DPAD_LEFT(bool*);	// Shop

			static void LB(bool*);			// Summoner Spell 1 - (B - Recall if inverted)

			static void RB(bool*);			// Summoner Spell 2 - (Move - Right click if inverted)

			static void START(bool*);		// ESC  - Opens in-game Start menu/Quick-close menus

			static void BACK(bool*);		// TAB - Stat Screen

			static void THUMB_L(bool*);		// T - Emote

			static void THUMB_R(bool*);		// Left click

		private:
			static void keyInput(bool*,		// Helper function - simulates keypresses on keyboard using scancodes
				const WORD&);

			static void mouseInput(bool*,	// Helper function - simulates mouse button clicks
				const WORD&,
				const WORD&);
		};
	};
}