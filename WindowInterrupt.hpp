//-------------------------------------------------------------------
#ifndef __windowInterrupt_H__
#define __windowInterrupt_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowInterrupt : public Window
{
protected:
	byte secondLine;

public:
	WindowInterrupt(byte inFirstLine, byte inSecondLine, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_INTERRUPT; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
