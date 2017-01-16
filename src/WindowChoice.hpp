//-------------------------------------------------------------------
#ifndef __windowChoice_H__
#define __windowChoice_H__
//-------------------------------------------------------------------

#include "Window.hpp"
#include "Chain.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

struct Choice
{
	byte id;	// string id
	byte index;
	byte escapeWindowId;	// escape window id or 255
};

// Select a choice in the list created by AddChoice() calls.

/// +------------------+
/// | Choose something |
/// |     A choice     |
/// |    >B choice<    |
/// |     C choice     |
/// +------------------+

class WindowChoice : public Window
{
protected:
	Choice *pSelectedChoice;	// selected choice index

	LCDUICHAINEDLIST<Choice> Choices;

public:
	WindowChoice(byte inFirstLine, Choice *inpSelectedChoice, bool inUseIndexes = false, int inTag = 0);

	inline byte GetType() const { return WINDOWTYPE_CHOICE; }
	inline const Choice &GetSelectedChoice() const { return *this->pSelectedChoice; }
	inline byte GetSelectedChoiceId() const { return pSelectedChoice->id; }
	inline byte GetSelectedChoiceIndex() const { return pSelectedChoice->index; }
	inline byte GetSelectedChoiceEscapeWindowId() { return this->pSelectedChoice->escapeWindowId; }

	void MoveNextChoice();
	void MovePreviousChoice();

	byte AddChoice(byte inStringIndex, byte inIndex = 255, byte inInterruptOnEscape = 255);

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
