//-------------------------------------------------------------------
#ifndef __windowYesNo_H__
#define __windowYesNo_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// change a boolean value

/// +------------------+
/// | Lights on ?      |
/// |     Yes >No<     |
/// |                  |
/// |                  |
/// +------------------+

class WindowYesNo : public Window
{
private:
	bool *pValue;

public:
	WindowYesNo(byte inFirstLine, bool *inpValue, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_YESNO; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
