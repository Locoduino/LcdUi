//-------------------------------------------------------------------
#ifndef __windowSplash_H__
#define __windowSplash_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// This is a temporary window for time limited messages

/// +------------------+
/// | LCD UI v0.30     |
/// | (c) Locoduino    |
/// |                  |
/// |                  |
/// +------------------+


class WindowSplash : public Window
{
protected:
	byte secondLine;
	unsigned long delay;
	unsigned long startingDate;

public:
	WindowSplash(byte inFirstLine, byte inSecondLine, unsigned long inDelay = 2000, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_SPLASH; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
