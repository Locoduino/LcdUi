//-------------------------------------------------------------------
#ifndef __windowInterruptConfirm_H__
#define __windowInterruptConfirm_H__
//-------------------------------------------------------------------

#include "WindowInterrupt.hpp"

//-------------------------------------------------------------------

class WindowInterruptConfirm : public WindowInterrupt
{
private:
	// values for edition modes
	byte choiceValue;

public:
	WindowInterruptConfirm(byte inFirstLine, byte prefix);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }
	inline byte GetChoiceValue() const { return this->choiceValue; }
	inline void SetValue(byte inValue) { this->choiceValue = inValue; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
