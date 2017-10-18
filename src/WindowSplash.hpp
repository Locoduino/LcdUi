//-------------------------------------------------------------------
#ifndef __windowSplash_H__
#define __windowSplash_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// This is a temporary window for time limited messages

/// +------------------+
/// | LCD UI v1.00     |
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
	inline WindowSplash() { this->secondLine = 0; this->delay = 0; this->startingDate = 0; }
	inline WindowSplash(byte inFirstLine, byte inSecondLine, unsigned long inDelay = 2000) { this->begin(inFirstLine, inSecondLine, inDelay); }

	void begin(byte inFirstLine, byte inSecondLine, unsigned long inDelay = 2000);
	inline byte GetType() const { return WINDOWTYPE_SPLASH; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
