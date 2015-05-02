//-------------------------------------------------------------------
#ifndef __windowYesNo_H__
#define __windowYesNo_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

//-------------------------------------------------------------------

class WindowYesNo : public Window
{
private:
	// strings for choice mode.
	byte choiceValue, memoChoiceValue;	// string number as stored in 'choices', not the index !

public:
	WindowYesNo(byte inFirstLine) : Window(inFirstLine) { }

	inline byte GetType() const { return WINDOWTYPE_YESNO; }
	inline byte GetChoiceValue() const { return this->choiceValue; }
	inline void SetChoiceValue(byte inValue) { this->choiceValue = inValue; }

	inline void MemoChoiceValue() { this->memoChoiceValue = this->choiceValue; }
	inline void RestoreChoiceValue() { this->choiceValue = this->memoChoiceValue; }

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
