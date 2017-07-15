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
/// +------------------+
/// | Choose something |
/// |     A choice     |
/// |    >B choice<    |
/// |     C choice     |
/// +------------------+

class LcdScreen;

class WindowChoiceText : public Window
{
protected:
	// current selected index
	byte *pValue;

public:
	inline WindowChoiceText() { this->pValue = NULL; this->lockScreen = false; }
	inline WindowChoiceText(byte inFirstLine, byte *inpValue) { this->begin(inFirstLine, inpValue);	}

	void begin(byte inFirstLine, byte *inpValue);
	inline void SetValueAddress(byte *inpValue) { this->pValue = inpValue; }
	inline byte GetType() const { return WINDOWTYPE_CHOICETEXT; }
	inline unsigned int GetChoiceValue() const { return *(this->pValue); }
	void SetCurrentChoice(byte inChoiceValue);
	inline virtual void MoveNextChoice() { this->Move(true); }
	inline virtual void MovePreviousChoice() { this->Move(false); }
	virtual void Move(bool inMore);
	void Event(byte inEventType, LcdUi *inpLcd);

	// Abstract functions the derived class must instanciate
	virtual byte GetChoiceTextNumber() const = 0;
	virtual char *GetChoiceTextValue(byte indexValue, LcdScreen *apScreen) const = 0;

	bool lockScreen;

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
