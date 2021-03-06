//-------------------------------------------------------------------
#ifndef __LcdUi_H__
#define __LcdUi_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define LCDUI_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if LCDUI_DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

///////////////////////////////////////////////////////
// The function LcdUi::printWindows()
// is very heavy in program memory. So to avoid problems
// you can make this function available by uncomment the next 
// line.
//#define LCDUI_PRINT_WINDOWS

/*#ifdef VISUALSTUDIO
#include "Arduino.h"
#include "string.h"
#define STRNCPY(out, len ,in)	strcpy_s(out, len, in)
#define STRCPY(out, in)			strcpy_s(out, in)
#else
#define STRNCPY(out, len ,in)	strcpy(out, in)
#define STRCPY(out, in)			strcpy(out, in)
#endif*/

#include "Arduino.h"

// GetNextWindow already exists in system includes sdk/winuser.h...
#ifdef GetNextWindow
#undef GetNextWindow
#endif

#define STRNCPY(out, len ,in)	strcpy(out, in)
#define STRCPY(out, in)			strcpy(out, in)

#ifndef STR_LCDTITLE
#define STR_LCDTITLE16		F("LCD UI V1.0.2")
#define STR_LCDCOPYRIGHT16	F("By Thierry Paris")

#define STR_LCDTITLE		F("LCD User Interface V1.0.2")
#define STR_LCDCOPYRIGHT	F("Developed by Thierry Paris.")
#endif

//////////////////////////////////////////
//  Exclusion area
//
// You can exclude some parts of library here, to avoid losing program and data memory
// on parts you dont use.
// For example, if you dont want buttons commander
// just uncomment the line #define NO_BUTTONSCOMMANDER by removing // at the beggining.
//
// Arduino IDE build of list of the files to compile from the content of the library directory
// in a makefile and give it to Gcc. So Gcc compiles all the files of the directory
// even if its content is not used. The result is, if an unused source file contains
// static declarations, these statics will be allocated a this memory will be lost.
// The only solution I have found is to rename the source files to something IDE and Gcc dont know...
//
// So if you want to lose less memory, you can rename the linked files
// from .cpp to .cpp.ori, and from .hpp to .hpp.ori.
// See below the file names related to each exclusion:
//

/////////////////////////////////////

#include "Window.hpp"
#include "WindowChoice.hpp"
#include "WindowChoiceText.hpp"
#include "WindowConfirm.hpp"
#include "WindowInt.hpp"
#include "WindowText.hpp"
#include "WindowYesNo.hpp"
#include "WindowInterrupt.hpp"
#include "WindowInterruptConfirm.hpp"
#include "WindowSplash.hpp"
#include "WindowSequence.hpp"

#include "LcdScreen.hpp"

#ifdef LiquidCrystal_h
#include "ScreenLiquid.hpp"
#endif

#ifdef _LCD_H_
#include "ScreenLiquidNew.hpp"
#endif

#ifdef _ADAFRUIT_PCD8544_H
#include "ScreenNokia5110.hpp"
#endif

#define EVENT_NONE		0
#define EVENT_MORE		1
#define EVENT_LESS		2
#define EVENT_SELECT	3
#define EVENT_CANCEL	4
#define EVENT_MOVE		5
#define EVENT_START		6
#define EVENT_END		7

class LcdUi
{
private:
	static bool firstLoop;

	LcdScreen *pScreen;
	Window *pFirstWindow;

	Window *pCurrentWindow;
	Window *pWindowInterrupt;
	Window *pWindowSequence;
	
	// Functions used by interactive mode to evoluate in the UI
	Window *GetNextWindowWithFather(Window *inpCurr, byte inFatherToFindId, byte inChoiceToFind = 255);
	void MoveToNextUIWindow();
	void MoveToPrevUIWindow();

public:
	LcdUi();

	void begin(LcdScreen *inpScreen);
	Window *AddWindow(Window *inpWindow);
	void MoveToWindow(Window *inpWindow);

	bool loop(byte inEvent);
	byte InterruptByEvent(byte inEventType);
	void Interrupt(Window *inpInterrupt);
	void Interrupt(byte inWindowId);
	void InterruptEnd();
	inline Window *GetFirstWindow() const { return this->pFirstWindow; }
	inline Window *GetCurrentWindow() const { return this->pCurrentWindow; }
	inline Window *GetWindowInterrupt() { return this->pWindowInterrupt; }
	inline Window *GetGlobalCurrentWindow() const { return this->pWindowInterrupt != NULL ? this->pWindowInterrupt : this->pCurrentWindow; }
	inline byte GetGlobalCurrentWindowId() const { return this->GetGlobalCurrentWindow()->GetFirstLine(); }
	inline LcdScreen *GetScreen() const { return this->pScreen; }

	Window *GetPreviousWindow(Window *inpPrevious) const;
	Window *GetWindowById(byte inId) const;
	inline byte GetWindowId() const { return this->GetGlobalCurrentWindow()->GetWindowId(); }
	inline byte GetState() const { return this->GetGlobalCurrentWindow()->GetState(); }
	inline void SetState(byte inState) { this->GetGlobalCurrentWindow()->SetState(inState); }
	inline byte GetType() const { return this->GetGlobalCurrentWindow()->GetType(); }
	bool IsValueModified(byte inWindowId);

	inline byte GetChoiceValue() const { return this->GetGlobalCurrentWindow()->GetFatherChoiceValue(); }
	inline void SetChoiceValue(byte inValue) { this->GetGlobalCurrentWindow()->SetFatherChoiceValue(inValue); }

#ifdef LCDUI_DEBUG_MODE
public:
	static void printEvent(byte inEvent, const __FlashStringHelper *inFunc);
#endif
#ifdef LCDUI_PRINT_WINDOWS
	#define PRINT_WINDOWS(pLCDUI)		pLCDUI->printWindows();
	void printWindows();
#else
	#define PRINT_WINDOWS(pLCDUI)
#endif
};
#endif