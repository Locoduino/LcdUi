//-------------------------------------------------------------------
#ifndef __windowText_H__
#define __windowText_H__
//-------------------------------------------------------------------

#include "Window.hpp"
//#include "string.h"

//-------------------------------------------------------------------

#define WINDOW_MAXTEXTVALUESIZE	20

#define STATE_SELECTCHAR	100
#define STATE_SELECTBACK	101

class WindowText : public Window
{
private:
	byte maxTextValueLength;
	byte currentCharPos;
	byte currentCharEdited;
	char textValue[WINDOW_MAXTEXTVALUESIZE + 1];

public:
	WindowText(byte inFirstLine, byte inMaxLengthValue = WINDOW_MAXTEXTVALUESIZE, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_TEXT; }
	inline byte GetMaxTextValueLength() const { return this->maxTextValueLength; }
	inline void SetMaxTextValueLength(byte inLength) { this->maxTextValueLength = inLength; }

	inline const char *GetTextValue() const { return this->textValue; }
	inline void SetValue(const char *inValue) { STRCPY(this->textValue, inValue); }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
