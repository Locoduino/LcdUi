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
	byte answer;

public:
	inline WindowConfirm() { this->prefix = 0; this->answer = 0; }
	inline WindowConfirm(byte inFirstLine, byte prefix) { this->begin(inFirstLine, prefix); }

	void begin(byte inFirstLine, byte prefix);
	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }
	inline byte GetValue() const { return this->answer; }
	inline void SetValue(byte inAnswer) { this->answer = inAnswer; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
