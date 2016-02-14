//-------------------------------------------------------------------
#ifndef __windowYesNo_H__
#define __windowYesNo_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowYesNo : public Window
{
public:
	WindowYesNo(byte inFirstLine, int inTag = 0) : Window(inFirstLine, inTag) { }

	inline byte GetType() const { return WINDOWTYPE_YESNO; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------