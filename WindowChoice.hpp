//-------------------------------------------------------------------
#ifndef __windowChoice_H__
#define __windowChoice_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

class WindowChoice : public Window
{
private:
	byte choiceAddCounter;

	// strings for choice mode.
	byte choices[WINDOW_MAXCHOICES];
	byte escapeWindows[WINDOW_MAXCHOICES];

public:
	WindowChoice(byte inFirstLine);

	inline byte GetType() const { return WINDOWTYPE_CHOICE; }
	byte GetChoiceIndex() const;
	inline byte *GetChoices() { return this->choices; }
	inline byte GetChoiceEscapeWindow() { return this->escapeWindows[this->GetChoiceIndex()]; }
	void MoveNextChoice();
	void MovePreviousChoice();

	void AddChoice(byte inStringIndex, byte inInterruptOnEscape = 255);

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
