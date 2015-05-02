//-------------------------------------------------------------------
#ifndef __windowSplash_H__
#define __windowSplash_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowSplash : public Window
{
protected:
	byte secondLine;
	unsigned long delay;
	unsigned long startingDate;

public:
	WindowSplash(byte inFirstLine, byte inSecondLine, unsigned long inDelay = 2000);

	inline byte GetType() const { return WINDOWTYPE_SPLASH; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
