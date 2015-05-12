//-------------------------------------------------------------------
#ifndef __windowInterruptConfirm_H__
#define __windowInterruptConfirm_H__
//-------------------------------------------------------------------

#include "WindowInterrupt.hpp"

//-------------------------------------------------------------------

class WindowInterruptConfirm : public WindowInterrupt
{
public:
	WindowInterruptConfirm(byte inFirstLine, byte prefix);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
