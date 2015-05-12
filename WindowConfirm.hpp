//-------------------------------------------------------------------
#ifndef __windowConfirm_H__
#define __windowConfirm_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowConfirm : public Window
{
private:
	byte prefix;

public:
	WindowConfirm(byte inFirstLine, byte prefix);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
