//-------------------------------------------------------------------
#ifndef __windowLocoControl_H__
#define __windowLocoControl_H__
//-------------------------------------------------------------------

#define WINDOWTYPE_LOCOCONTROL	100

class WindowLocoControl : public Window
{
private:
	int msg;

public:
	WindowLocoControl(int inFirstLine);

	inline byte GetType() const { return WINDOWTYPE_LOCOCONTROL; }
	void Event(byte inEventType, LcdUi *inpLcd);
};

#endif
//-------------------------------------------------------------------
