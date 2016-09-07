//-------------------------------------------------------------------
#ifndef __windowYesNo_H__
#define __windowYesNo_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowYesNo : public Window
{
private:
	bool *pValue;

public:
	WindowYesNo(byte inFirstLine, bool *inpValue, int inTag = 0) : Window(inFirstLine, inTag) { this->pValue = inpValue; }

	inline byte GetType() const { return WINDOWTYPE_YESNO; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
