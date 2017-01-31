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
	inline WindowInterruptConfirm() { this->answer = 0; }
	inline WindowInterruptConfirm(byte inFirstLine, byte inSecondLine) { this->begin(inFirstLine, inSecondLine); }

	void begin(byte inFirstLine, byte inSecondLine);
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
