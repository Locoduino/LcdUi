//-------------------------------------------------------------------
#ifndef __windowInterrupt_H__
#define __windowInterrupt_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

class WindowInterrupt : public Window
{
protected:
	byte secondLine;

public:
	WindowInterrupt(byte inFirstLine, byte inSecondLine);

	inline byte GetType() const { return WINDOWTYPE_INTERRUPT; }

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
