//-------------------------------------------------------------------
#ifndef __windowChoice_H__
#define __windowChoice_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

#define WINDOW_MAXCHOICES		10

class WindowChoice : public Window
{
private:
	byte choiceAddCounter;

	// strings for choice mode.
	byte choiceValue, memoChoiceValue;	// string number as stored in 'choices', not the index !
	byte choices[WINDOW_MAXCHOICES];
	byte escapeWindows[WINDOW_MAXCHOICES];

public:
	WindowChoice(byte inFirstLine);

	inline byte GetType() const { return WINDOWTYPE_CHOICE; }
	inline byte GetChoiceValue() const { return this->choiceValue; }
	byte GetChoiceIndex() const;
	inline byte *GetChoices() { return this->choices; }
	inline byte *GetEscapeWindows() { return this->escapeWindows; }
	void MoveNextChoice();
	void MovePreviousChoice();
	inline void SetChoiceValue(byte inValue) { this->choiceValue = inValue; }

	inline void MemoChoiceValue() { this->memoChoiceValue = this->choiceValue; }
	inline void RestoreChoiceValue() { this->choiceValue = this->memoChoiceValue; }

	void AddChoice(byte inStringIndex, byte inInterruptOnEscape = 255);

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------

//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their  
// 		declarations into this file.
// 		If you do not want to use this automated process,  
//		simply delete the lines below, with "&MM_DECLA" text 
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
