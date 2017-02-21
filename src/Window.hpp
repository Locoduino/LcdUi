//-------------------------------------------------------------------
#ifndef __window_H__
#define __window_H__
//-------------------------------------------------------------------

#include "arduino.h"

#define MAXSTRINGVALUE	20

#define WINDOWTYPE_YESNO				0
#define WINDOWTYPE_CONFIRM				1	
#define WINDOWTYPE_CHOICE				2
#define WINDOWTYPE_INT					3
#define WINDOWTYPE_TEXT					4
#define WINDOWTYPE_INTERRUPT			5
#define WINDOWTYPE_INTERRUPTCONFIRM		6
#define WINDOWTYPE_SPLASH				7
#define WINDOWTYPE_CHOICETEXT			8

/*
Steps of the life :

	STATE_START		Show first line
	STATE_POSTSTART	initialize value if necessary

	STATE_LOOP		Waiting for events

		Escape :	STATE_ABORTED		Return to previous window.

		Confirm :	STATE_CONFIRMED		Get new value and use it
					STATE_POSTCONFIRMED	Go to the next window

*/

#define STATE_NONE				0
#define STATE_START				1
#define STATE_INITIALIZE		2
#define STATE_LOOP				3
#define STATE_ABORTED			4
#define STATE_CONFIRMED			5
#define STATE_POSTCONFIRMED		6

//-------------------------------------------------------------------

class LcdUi;

class Window
{
protected:
	byte firstLine;		// id
	byte state;
	bool active;
	int tag;			// User data...
	Window *pNextWindow;

	// A window can have a father window.
	// If this father is a WindowChoice, each choice can go to a Windows list for THIS choice.
	// For instance, you select 'B' in a multiple choice list, and 'B' give acces to three more windows...
	// Each of these sub windows will say : my father is the choice, 
	// and I will be called if choice 'B' is activated.
	Window *pFatherWindow;
	byte fatherChoiceValue;	// Index of the choice, 255 if undefined

public:
	Window();
	inline Window(byte inFirstLine) : Window::Window() { this->begin(inFirstLine); }

	void begin(byte inFirstLine);
	inline byte GetWindowId() { return this->GetFirstLine(); }
	inline byte GetFirstLine() const { return this->firstLine; }
	inline byte GetState() const { return this->state; }
	inline int GetTag() const { return this->tag; }
	inline bool IsActive() const { return this->active; }
	inline void SetFirstLine(byte inLine) { this->firstLine = inLine; }
	inline void SetState(byte inState) { this->state = inState; }
	inline void SetTag(int inTag) { this->tag = inTag; }
	inline void SetActive(bool inActive) { this->active = inActive; }

	inline virtual byte GetType() const { return 255; }
	inline virtual void Event(byte inEventType, LcdUi *inpLcd) {}

	inline void SetNextWindow(Window *inWindow) { this->pNextWindow = inWindow; }
	inline Window *GetNextWindow() const { return this->pNextWindow; }

	inline void SetFatherWindow(Window *inpFatherWindow) { this->pFatherWindow = inpFatherWindow; }
	inline Window *GetFatherWindow() const { return this->pFatherWindow; }
	inline byte GetFatherChoiceValue() const { return this->fatherChoiceValue; }
	inline void SetFatherChoiceValue(byte inValue) { this->fatherChoiceValue = inValue; }
	inline void SetFather(Window *inpFatherWindow, byte inValue) { this->pFatherWindow = inpFatherWindow; this->fatherChoiceValue = inValue; }

#ifdef LCDUI_DEBUG_MODE
	static void printState(byte inState, const __FlashStringHelper *inFunc);
#endif

#ifdef LCDUI_PRINT_WINDOWS
	virtual void printWindow() {};
	void printWindowHeader(const __FlashStringHelper *inName);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
