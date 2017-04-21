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
	byte currentConfiguration;

	LCDUICHAINEDLIST<Choice> Choices;

public:
	inline WindowChoice() { this->pSelectedChoice = NULL; }
	inline WindowChoice(byte inFirstLine, Choice *inpSelectedChoice) { this->begin(inFirstLine, inpSelectedChoice); }

	void begin(byte inFirstLine, Choice *inpSelectedChoice);
	inline byte GetType() const { return WINDOWTYPE_CHOICE; }
	inline const Choice &GetSelectedChoice() const { return *this->pSelectedChoice; }
	inline byte GetSelectedChoiceId() const { return pSelectedChoice->id; }
	inline byte GetSelectedChoiceIndex() const { return pSelectedChoice->index; }
	inline byte GetSelectedChoiceEscapeWindowId() { return this->pSelectedChoice->escapeWindowId; }
	inline byte GetCurrentConfiguration() const { return this->currentConfiguration; }
	inline void SetCurrentConfiguration(byte inNewConfiguration) { this->currentConfiguration = inNewConfiguration; }
	void SetCurrentChoiceById(byte inId);

	void MoveNextChoice();
	void MovePreviousChoice();

	byte AddChoice(byte inStringNumber, Window *apChildWindow = NULL, byte inIndex = 255, byte inInterruptIdOnEscape = 255);

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
