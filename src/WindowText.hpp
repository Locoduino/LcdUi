//-------------------------------------------------------------------
#ifndef __windowText_H__
#define __windowText_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// Changes a text value

/// +------------------+
/// | LOCODUINO___     |
/// |        >Z<       |
/// |                  |
/// |                  |
/// +------------------+

#define WINDOW_MAXTEXTVALUESIZE	20

#define STATE_SELECTCHAR	100
#define STATE_SELECTBACK	101

class WindowText : public Window
{
private:
	byte maxTextValueLength;
	byte currentCharPos;
	byte currentCharEdited;
	char *pTextValue;

public:
	static byte Backspace;
	inline WindowText() { this->maxTextValueLength = WINDOW_MAXTEXTVALUESIZE; this->currentCharPos = 0; this->currentCharEdited = 0; this->pTextValue = NULL; }
	inline WindowText(byte inFirstLine, char *inpValue, byte inMaxLengthValue = WINDOW_MAXTEXTVALUESIZE) { this->begin(inFirstLine, inpValue, inMaxLengthValue); }

	void begin(byte inFirstLine, char *inpValue, byte inMaxLengthValue = WINDOW_MAXTEXTVALUESIZE);
	inline byte GetType() const { return WINDOWTYPE_TEXT; }
	inline byte GetMaxTextValueLength() const { return this->maxTextValueLength; }
	inline void SetMaxTextValueLength(byte inLength) { this->maxTextValueLength = inLength; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
