//-------------------------------------------------------------------
#ifndef __windowLocoControl_H__
#define __windowLocoControl_H__
//-------------------------------------------------------------------
#include "LcdUi.h"
//-------------------------------------------------------------------

#define WINDOWTYPE_LOCOCONTROL	14

class WindowLocoControl : public Window
{
public:
	bool isDcc;
	byte dcFrequency;
	const char *dcFrequencyText;
	int Address;
	byte AddressSize;
	byte Speed;
	byte SpeedMax;
	int Speed128Inc;	// int only because of the editing window which accepts only ints...
	bool Direction;
	char Name[12];

public:
	WindowLocoControl();

	inline byte GetType() const { return WINDOWTYPE_LOCOCONTROL; }
	void Event(byte inEventType, LcdUi *inpLcd);

	void printWindow() { Serial.println("WindowLocoControl"); }
};

#endif
//-------------------------------------------------------------------
