//-------------------------------------------------------------------
#ifndef __windowChoiceText_H__
#define __windowChoiceText_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

//
// This windows can choose an item inside a  list of strings, and return the 
// selected string or the associated integer value.
//
class WindowChoiceText : public Window
{
protected:
	// strings for choice mode.
	char *currentValueText;
	unsigned int currentValue;

public:
	WindowChoiceText(byte inFirstLine, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CHOICETEXT; }
	inline unsigned int GetChoiceIntValue() const { return this->currentValue; }
	inline char *GetChoiceTextValue() const { return this->currentValueText; }
	void SetCurrentChoice(char *inChoiceText, unsigned int inChoiceValue);
	virtual void MoveNextChoice() {}
	virtual void MovePreviousChoice() {}

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
