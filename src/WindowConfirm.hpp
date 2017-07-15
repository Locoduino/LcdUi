//-------------------------------------------------------------------
#ifndef __windowConfirm_H__
#define __windowConfirm_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// A yes/no window with a text prefix.

/// +------------------+
/// | Reset config     |
/// | Sure ? Yes >No<  |
/// |                  |
/// |                  |
/// +------------------+

class WindowConfirm : public Window
{
private:
	byte prefix;
	bool *pValue;

public:
	inline WindowConfirm() { this->prefix = 0; this->pValue = NULL; }
	inline WindowConfirm(byte inFirstLine, byte inPrefix, bool *inpValue) { this->begin(inFirstLine, inPrefix, inpValue); }

	void begin(byte inFirstLine, byte prefix, bool *inpValue);
	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
