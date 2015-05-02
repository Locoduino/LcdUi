//-------------------------------------------------------------------
#ifndef __windowConfirm_H__
#define __windowConfirm_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowConfirm : public Window
{
private:
	// values for edition modes
	byte choiceValue;
	byte prefix;

public:
	WindowConfirm(byte inFirstLine, byte prefix);

	inline byte GetType() const { return WINDOWTYPE_CONFIRM; }
	inline byte GetChoiceValue() const { return this->choiceValue; }
	inline void SetValue(byte inValue) { this->choiceValue = inValue; }

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
