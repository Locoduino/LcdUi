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
	byte CurrentWindow;
	byte WindowInterrupt;
	
	// functions to move in the windows list
	byte GetParentWindow(byte inRef);
	byte GetNextChildWindow(byte inRef);

	// Functions used by interactive mode to evolute in the UI
	void GetNextUIWindow();
	void GetPrevUIWindow();

public:
	LcdUi();

	void Setup(Screen *inpScreen, int inNbWindows = 0);
	void SetWindowsNumber(int inNbWindows);
	Window *AddWindow(Window *inpWindow, Window *inpFatherWindow = 0, byte inChoiceNumber = 255);
	inline void SetWindow(byte inWindow) { this->CurrentWindow = inWindow; }

	static void StartSetup();
	static void EndSetup();

	bool Loop(byte inEvent);
	void Interrupt(byte inInterrupt);
	void InterruptEnd();
	inline byte GetCurrentWindow() const { return this->CurrentWindow; }
	inline byte GetWindowInterrupt() { return this->WindowInterrupt; }
	inline Window *GetGlobalCurrentWindow() const { return this->WindowInterrupt != 255 ? this->pWindows[this->WindowInterrupt] : this->pWindows[this->CurrentWindow]; }
	inline Screen *GetScreen() const { return this->pScreen; }

	byte GetWindowIndex(Window *inpWindow) const;
	byte GetWindowById(byte inId) const;
	inline byte GetFather(int inChildIndex) const { return (pNodeFather[inChildIndex] / 100) == 0 ? 255 : (pNodeFather[inChildIndex] / 100) - 1; }
	inline byte GetFatherChoice(int inChildIndex) const { return (pNodeFather[inChildIndex] % 100) == 0 ? 255 : (pNodeFather[inChildIndex] % 100) - 1; }
	inline byte GetWindowId() const { return this->GetGlobalCurrentWindow()->GetWindowId(); }
	inline byte GetState() const { return this->GetGlobalCurrentWindow()->GetState(); }
	inline void SetState(byte inState) { this->GetGlobalCurrentWindow()->SetState(inState); }
	inline byte GetType() const { return this->GetGlobalCurrentWindow()->GetType(); }

	inline byte GetChoiceValue() const { return this->GetGlobalCurrentWindow()->GetChoiceValue(); }
	inline void SetChoiceValue(byte inValue) { this->GetGlobalCurrentWindow()->SetChoiceValue(inValue); }
	inline int GetIntValue() const { return this->GetGlobalCurrentWindow()->GetIntValue(); }
	inline const char *GetTextValue() const { return this->GetGlobalCurrentWindow()->GetTextValue(); }
	inline void SetValue(int inValue) { this->GetGlobalCurrentWindow()->SetValue(inValue); }
	inline void SetValue(const char *inValue) { this->GetGlobalCurrentWindow()->SetValue(inValue); }

#ifdef DEBUG_MODE
public:
	void CheckAddIndex(int inIndex, const __FlashStringHelper *infunc);
	void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
	static void printEvent(byte inEvent, const __FlashStringHelper *inFunc);
#endif
};

#define BEGIN_UI(lui, screen, nb)					LcdUi *plui = &lui;	plui->Setup(&screen, nb);
#define WIN											Window *
#define WINDOWCHOICE(first, ...)					plui->AddWindow(new WindowChoice(first), ##__VA_ARGS__);
#define ADDCHOICE(win, text, ...)					((WindowChoice *)win)->AddChoice(text);
#define WINDOWCONFIRM(first, prefix, ...)			plui->AddWindow(new WindowConfirm(first, prefix), ##__VA_ARGS__)
#define WINDOWINT(first, max, min, ...)				plui->AddWindow(new WindowInt(first, max, min), ##__VA_ARGS__)
#define WINDOWINTERRUPT(first, second, ...)			plui->AddWindow(new WindowInterrupt(first, second), ##__VA_ARGS__)
#define WINDOWINTERRUPTCONFIRM(first, prefix, ...)	plui->AddWindow(new WindowInterruptConfirm(first, prefix), ##__VA_ARGS__)
#define WINDOWSPLASH(first, second, delay, ...)		plui->AddWindow(new WindowSplash(first, second, delay), ##__VA_ARGS__)
#define WINDOWTEXT(first, len, ...)					plui->AddWindow(new WindowText(first, len), ##__VA_ARGS__)
#define WINDOWYESNO(first, ...)						plui->AddWindow(new WindowYesNo(first), ##__VA_ARGS__)
#define WINDOWCUSTOM(win, ...)						plui->AddWindow(win, ##__VA_ARGS__)
#define END_UI()
