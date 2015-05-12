//-------------------------------------------------------------------
#define __LcdUi_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

#define GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#include "VStudioMfc/arduino.h"
#include "VStudioMfc/Serial.hpp"
#define STRNCPY(out, len ,in)	strcpy_s(out, len, in)
#define STRCPY(out, in)			strcpy_s(out, in)
#else
#define STRNCPY(out, len ,in)	strcpy(out, in)
#define STRCPY(out, in)			strcpy(out, in)
#endif

#ifdef DEBUG_MODE
#define CHECKPIN(val, text)		CheckPinNb(cval, text)
#else
#define CHECKPIN(val, text)
#endif

#ifndef STR_LCDTITLE
#define STR_LCDTITLE16		F("LCD UI V0.1")
#define STR_LCDCOPYRIGHT16	F("By Thierry Paris")

#define STR_LCDTITLE		F("LCD User Interface V0.1")
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
#include "WindowConfirm.hpp"
#include "WindowInt.hpp"
#include "WindowText.hpp"
#include "WindowYesNo.hpp"
#include "WindowInterrupt.hpp"
#include "WindowInterruptConfirm.hpp"
#include "WindowSplash.hpp"

#include "Screen.hpp"
#include "ScreenTwoLines.hpp"

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
	Screen *pScreen;
	Window* *pWindows;
	/* (Index of the father Window in the Window list + 1) * 100 + (Choice number + 1 or 0)
									NodeFather			Comment
	0	Win A						0					no father...
	1		Win B choices 0,1,2		100					Win A (index 0+1) is the father.
	2			Win C				201					Win B/Choice 0 if the father (Index 1+1 * 100) + Choice (0+1)
	3			Win D				202					Win B/Choice 1 if the father (Index 1+1 * 100) + Choice (1+1)
	4			Win E				203					Win B/Choice 2 if the father (Index 1+1 * 100) + Choice (2+1)
	5			Win F				203					Win B/Choice 2 if the father (Index 1+1 * 100) + Choice (2+1)

	*/
	int *pNodeFather;
	int windowSize;
	int windowAddcounter;
	Window *pCurrentWindow;
	WindowInterrupt *pWindowInterrupt;
	
	// functions to move in the windows list
	int GetWindowIndex(Window *inpRef);
	Window *GetWindow(byte inId);
	Window *GetParentWindow(Window *inpRef);
	Window *GetChildWindow(Window *inpRef, byte inChoice = 255);
	Window *GetNextChildWindow(Window *inpRef);
	Window *GetPrevSiblingWindow(Window *inpRef);

	// Functions used by interactive mode to evolute in the UI
	void GetNextUIWindow();
	void GetPrevUIWindow();

public:
	LcdUi();

	void Setup(Screen *inpScreen, int inNbWindows = 0);
	void SetWindowsNumber(int inNbWindows);
	Window *AddWindow(Window *inpWindow, Window *inpFatherWindow = 0, byte inChoiceNumber = 255);
	inline void SetWindow(Window *inpWindow) { this->pCurrentWindow = inpWindow; }

	static void StartSetup();
	static void EndSetup();

	bool Loop(byte inEvent);
	void Interrupt(WindowInterrupt *pInterrupt);
	void InterruptEnd();
	inline Window *GetCurrentWindow() { return this->pCurrentWindow; }
	inline WindowInterrupt *GetWindowInterrupt() { return this->pWindowInterrupt; }
	inline Screen *GetScreen() const { return this->pScreen; }

	inline Window *GetFather(Window *pWindow) { return GetFather(GetWindowIndex(pWindow)); }
	inline Window *GetFather(int inChildIndex) { return (pNodeFather[inChildIndex] / 100) == 0 ? 0 : pWindows[(pNodeFather[inChildIndex] / 100) - 1]; }
	inline byte GetFatherChoice(Window *pWindow) { return GetFatherChoice(GetWindowIndex(pWindow)); }
	inline byte GetFatherChoice(int inChildIndex) { return (pNodeFather[inChildIndex] % 100) == 0 ? 0 : (pNodeFather[inChildIndex] % 100) - 1; }
	inline byte GetWindowId() { return this->pWindowInterrupt != 0 ? this->pWindowInterrupt->GetWindowId() : this->pCurrentWindow->GetWindowId(); }
	inline byte GetState() const { return this->pWindowInterrupt != 0 ? this->pWindowInterrupt->GetState() : this->pCurrentWindow->GetState(); }
	inline void SetState(byte inState) { if (this->pWindowInterrupt != 0) this->pWindowInterrupt->SetState(inState); else this->pCurrentWindow->SetState(inState); }

#ifdef DEBUG_MODE
public:
	void CheckAddIndex(int inIndex, const __FlashStringHelper *infunc);
	void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};

