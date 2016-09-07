//-------------------------------------------------------------------
#ifndef __windowInterruptConfirm_H__
#define __windowInterruptConfirm_H__
//-------------------------------------------------------------------

#include "WindowInterrupt.hpp"

//-------------------------------------------------------------------

class WindowInterruptConfirm : public WindowInterrupt
{
private:
	byte answer;

public:
	WindowInterruptConfirm(byte inFirstLine, byte inSecondLine, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }
	inline byte GetValue() const { return this->answer; }
	inline void SetValue(byte inAnswer) { this->answer = inAnswer; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
