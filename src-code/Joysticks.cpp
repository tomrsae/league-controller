#include "Joysticks.h"

namespace XWin
{
	// Positions

	short Joysticks::L_getPos_x() const
	{
		short x = static_cast<short>(m_xgPad->sThumbLX / 3276.80f); // 327.680f for 1-100

		return x;
	}

	short Joysticks::L_getPos_y() const
	{
		short y = static_cast<short>(m_xgPad->sThumbLY / 3276.80f);
		return y;
	}

	short Joysticks::R_getPos_x() const
	{
		return static_cast<short>(m_xgPad->sThumbRX / 32768.0f);
	}

	short Joysticks::R_getPos_y() const
	{
		return static_cast<short>(m_xgPad->sThumbRY / 32768.0f);
	}

	// InDeadZone

	bool Joysticks::L_inDeadZone() const
	{
		if (m_xgPad->sThumbLX > LEFT_THUMB_DEADZONE ||
			m_xgPad->sThumbLX < -LEFT_THUMB_DEADZONE)
			return false;

		if (m_xgPad->sThumbLY > LEFT_THUMB_DEADZONE ||
			m_xgPad->sThumbLY < -LEFT_THUMB_DEADZONE)
			return false;

		return true;
	}

	bool Joysticks::R_inDeadZone() const
	{
		if (m_xgPad->sThumbRX > RIGHT_THUMB_DEADZONE ||
			m_xgPad->sThumbRX < -RIGHT_THUMB_DEADZONE)
			return false;

		if (m_xgPad->sThumbRY > RIGHT_THUMB_DEADZONE ||
			m_xgPad->sThumbRY < -RIGHT_THUMB_DEADZONE)
			return false;

		return true;
	}
}
