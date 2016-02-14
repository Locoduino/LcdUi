//-------------------------------------------------------------------
#ifndef __windowChoice_H__
#define __windowChoice_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

class WindowChoice : public Window
{
protected:
	byte choiceAddCounter;
	byte currentIndex;

	// strings for choice mode.
	byte *pChoices;
	byte *pIndexes;
	byte *pEscapeWindows;

public:
	WindowChoice(byte inFirstLine, int inNumberOfChoices, bool inEscapeWindows, bool inUseIndexes = false, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CHOICE; }
	byte GetChoiceIndex() const;
	inline byte *GetChoices() { return this->pChoices; }
	inline byte GetChoiceEscapeWindow() { return this->pEscapeWindows == 0 ? 255 : this->pEscapeWindows[this->GetChoiceIndex()]; }
	void MoveNextChoice();
	void MovePreviousChoice();

	byte AddChoice(byte inStringIndex, byte inIndex = 255, byte inInterruptOnEscape = 255);

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
