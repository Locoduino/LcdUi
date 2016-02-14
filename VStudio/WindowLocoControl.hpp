//-------------------------------------------------------------------
#ifndef __windowLocoControl_H__
#define __windowLocoControl_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOWTYPE_LOCOCONTROL	100

class WindowLocoControl : public Window
{
public:
	byte Speed;
	byte MoreLessIncrement;
	bool DirectionToLeft;

	byte MaxSpeed;
	int DccId;
	char Name[40];
	int dccMsg;

public:
	WindowLocoControl(byte inLevel, int inFirstLine, byte inMoreLessIncrement = 10);

	inline byte GetType() const { return WINDOWTYPE_LOCOCONTROL; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

#endif
//-------------------------------------------------------------------
