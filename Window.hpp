//-------------------------------------------------------------------
#ifndef __window_H__
#define __window_H__
//-------------------------------------------------------------------

#include "arduino.h"

#ifdef VISUALSTUDIO
#ifndef byte
#define byte unsigned char
#endif
#endif

#define MAXSTRINGVALUE	20

#define WINDOWTYPE_YESNO		0
#define WINDOWTYPE_CONFIRM		1
#define WINDOWTYPE_CHOICE		2
#define WINDOWTYPE_INT			3
#define WINDOWTYPE_TEXT			4
#define WINDOWTYPE_INTERRUPT	5
#define WINDOWTYPE_SPLASH		6

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

typedef void(*event)(byte inEventType, LcdUi *inpItem);

class Window
{
protected:
	byte firstLine;
	byte state;
	byte choiceValue;	// string number as stored in 'choices', not the index !
	int tag;			// User data...

public:
	Window(byte inFirstLine, int inTag = 0);

	inline byte GetWindowId() { return this->GetFirstLine(); }
	inline byte GetFirstLine() const { return this->firstLine; }
	inline byte GetState() const { return this->state; }
	inline int GetTag() const { return this->tag; }
	inline void SetFirstLine(byte inLine) { this->firstLine = inLine; }
	inline void SetState(byte inState) { this->state = inState; }

	inline virtual byte GetChoiceValue() const { return this->choiceValue; }
	inline virtual void SetChoiceValue(byte inValue) { this->choiceValue = inValue; }
	//inline virtual int GetIntValue() const { return 0; }
	//inline virtual void SetValue(int inValue) { }
	//inline virtual const char *GetTextValue() const { return 0; }
	//inline virtual void SetValue(const char *inValue) { }

	inline virtual byte GetType() const { return 255; }
	inline virtual void Event(byte inEventType, LcdUi *inpLcd) {}

#ifdef DEBUG_MODE
	static void printState(byte inState, const __FlashStringHelper *inFunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
