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
	byte answer;

public:
	WindowConfirm(byte inFirstLine, byte prefix, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }
	inline byte GetValue() const { return this->answer; }
	inline void SetValue(byte inAnswer) { this->answer = inAnswer; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
