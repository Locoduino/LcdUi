//-------------------------------------------------------------------
#ifndef __windowInterrupt_H__
#define __windowInterrupt_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// An interruption window will replace the current window, until EVENT_CANCEL is received.

/// +------------------+
/// | Shutdown !       |
/// | Cancel to avoid  |
/// |                  |
/// |                  |
/// +------------------+

class WindowInterrupt : public Window
{
protected:
	byte secondLine;
	byte eventType;

public:
	inline WindowInterrupt() { this->secondLine = 0; this->eventType = 0; }
	inline WindowInterrupt(byte inFirstLine, byte inSecondLine, byte inEventType = 255) { this->begin(inFirstLine, inSecondLine, inEventType); }

	void begin(byte inFirstLine, byte inSecondLine, byte inEventType = 255);
	inline byte GetType() const { return WINDOWTYPE_INTERRUPT; }
	inline byte GetEventType() const { return this->eventType; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
