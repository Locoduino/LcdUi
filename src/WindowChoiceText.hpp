//-------------------------------------------------------------------
#ifndef __windowChoiceText_H__
#define __windowChoiceText_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

//
// This windows can choose an item inside a list of strings, and return the 
// selected string or the associated integer value.
//
class WindowChoiceText : public Window
{
protected:
	// strings for choice mode.
	char *currentValueText;
	// current selected index
	byte *pValue;

public:
	WindowChoiceText(byte inFirstLine, byte *inpValue, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CHOICETEXT; }
	inline unsigned int GetChoiceIntValue() const { return *(this->pValue); }
	inline char *GetChoiceTextValue() const { return this->currentValueText; }
	void SetCurrentChoice(char *inChoiceText, byte inChoiceValue);
	inline void SetCurrentChoice(byte inChoiceValue) { this->SetCurrentChoice(this->GetChoiceTextValue(inChoiceValue), inChoiceValue); }
	inline virtual void MoveNextChoice() { this->Move(true); }
	inline virtual void MovePreviousChoice() { this->Move(false); }
	virtual void Move(bool inMore);
	void Event(byte inEventType, LcdUi *inpLcd);

	// Abstract functions the derived class must instanciate
	virtual byte GetChoiceTextNumber() const = 0;
	virtual char *GetChoiceTextValue(unsigned int indexValue) const = 0;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
