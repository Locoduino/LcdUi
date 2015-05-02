//-------------------------------------------------------------------
#ifndef __windowInt_H__
#define __windowInt_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowInt : public Window
{
private:
	int maxIntValue;
	int minIntValue;

	// values for edition modes
	int intValue;

public:
	WindowInt(byte inFirstLine, int inMaxIntValue = 999999, int inMinIntValue = 0);

	inline byte GetType() const { return WINDOWTYPE_INT; }
	inline int GetMaxIntValue() const { return this->maxIntValue; }
	inline int GetMinIntValue() const { return this->minIntValue; }
	inline void SetMaxIntValue(int inMax) { this->maxIntValue = inMax; }
	inline void SetMinIntValue(int inMin) { this->minIntValue = inMin; }

	inline int GetIntValue() const { return this->intValue; }
	inline void SetValue(int inValue) { this->intValue = inValue; }

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
