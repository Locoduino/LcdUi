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

/////////////////////////////////////

#include "Window.hpp"

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

#define WINDOW_MAXCHOICES		10
#define WINDOW_MAXTEXTVALUESIZE	20

class LcdUi
{
public:
	struct WindowItem
	{
		byte type:4;
		byte state:4;
		byte first;
		byte second;

		// Choices
		byte choiceValue_currentCharPos;
		byte choiceAddCounter_currentCharEdited;

		// strings for choice mode.
		byte *choices;// [WINDOW_MAXCHOICES];
		byte *escapeWindows;// [WINDOW_MAXCHOICES];

		// Text
		char *textValue; //[WINDOW_MAXTEXTVALUESIZE + 1];

		// values for edition modes
		int intValue_maxTextValueLength;

		// Splash + Int
		unsigned long delay_maxIntValue;
		unsigned long startingDate_minIntValue;

		Window *pCustom;
	};

private:
	Screen *pScreen;
	WindowItem *pWindows;

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
	byte GetWindow(byte inId);
	byte GetParentWindow(byte inRef);
	byte GetChildWindow(byte inRef, byte inChoice = 255);
	byte GetNextChildWindow(byte inRef);
	byte GetPrevSiblingWindow(byte inpRef);

	// Functions used by interactive mode to evolute in the UI
	void GetNextUIWindow();
	void GetPrevUIWindow();

public:
	LcdUi();

	void Setup(Screen *inpScreen, int inNbWindows = 0);
	void SetWindowsNumber(int inNbWindows);

	//Window *AddWindow(Window *inpWindow, Window *inpFatherWindow = 0, byte inChoiceNumber = 255);

	byte AddWindow(byte inType, byte infirst, byte inSecond, int inMax, int inMin, byte inMaxLength,
		unsigned long inDelay, byte inFatherWindow, byte inChoiceNumber);

	inline byte AddWindowChoice(byte inFirst, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_CHOICE, inFirst, 255, 9999, 0, 0, 0, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowConfirm(byte inFirst, byte prefix, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_CONFIRM, inFirst, prefix, 9999, 0, 0, 0, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowInt(byte inFirst, int inMax = 9999, int inMin = 0, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_INT, inFirst, 255, inMax, inMin, 0, 0, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowSplash(byte inFirst, byte inSecond, unsigned long delay = 0, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_SPLASH, inFirst, inSecond, 9999, 0, 0, delay, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowText(byte inFirst, byte length = WINDOW_MAXTEXTVALUESIZE, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_TEXT, inFirst, 255, 9999, 0, length, 0, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowYesNo(byte inFirst, byte inFatherWindow = 255, byte inChoiceNumber = 255)
	{
		return AddWindow(WINDOWTYPE_YESNO, inFirst, 255, 9999, 0, 0, 0, inFatherWindow, inChoiceNumber);
	}
	inline byte AddWindowInterrupt(byte inFirst, byte inSecond)
	{
		return AddWindow(WINDOWTYPE_INTERRUPT, inFirst, inSecond, 9999, 0, 0, 0, 255, 255);
	}

	byte AddWindowCustom(Window *inpCustom, byte inFatherWindow = 255, byte inChoiceNumber = 255);

	inline void SetWindow(byte inItem) { this->CurrentWindow = inItem; }

	static void StartSetup();
	static void EndSetup();

	bool Loop(byte inEvent);
	void Event(byte inEvent);
	void Interrupt(byte inInterrupt);
	void InterruptEnd();
	inline WindowItem &GetCurrentWindowItem() const { return this->pWindows[this->CurrentWindow]; }
	inline byte GetCurrentWindow() { return this->CurrentWindow; }
	inline byte GetWindowInterrupt() { return this->WindowInterrupt; }
	inline Screen *GetScreen() const { return this->pScreen; }

	inline byte GetFather(byte inChildIndex) const { return (pNodeFather[inChildIndex] / 100) == 0 ? 0 : (pNodeFather[inChildIndex] / 100) - 1; }
	inline byte GetFatherChoice(byte inChildIndex) const { return (pNodeFather[inChildIndex] % 100) == 0 ? 0 : (pNodeFather[inChildIndex] % 100) - 1; }
	inline byte GetWindowId() const { return this->WindowInterrupt != 255 ? this->pWindows[this->WindowInterrupt].first : this->GetCurrentWindowItem().first; }
	inline byte GetState() const { return this->WindowInterrupt != 255 ? this->pWindows[this->WindowInterrupt].state : this->GetCurrentWindowItem().state; }
	inline byte GetType() const { return this->WindowInterrupt != 255 ? this->pWindows[this->WindowInterrupt].type : this->GetCurrentWindowItem().type; }
	inline void SetState(byte inState) { if (this->WindowInterrupt != 255) this->pWindows[this->WindowInterrupt].state = inState; else this->GetCurrentWindowItem().state = inState; }

	/// Choice part
	byte GetChoiceIndex(byte inWindow = 255) const;
	inline byte *GetChoices() { return this->GetCurrentWindowItem().choices; }
	void MoveNextChoice();
	void MovePreviousChoice();
	void AddChoice(byte inOwner, byte inStringIndex, byte inInterruptOnEscape = 255);

	// Updating value
	inline byte GetChoiceValue() const { return this->GetCurrentWindowItem().choiceValue_currentCharPos; }
	inline void SetChoiceValue(byte inValue) { this->GetCurrentWindowItem().choiceValue_currentCharPos = inValue; }
	inline int GetIntValue() const { return this->GetCurrentWindowItem().intValue_maxTextValueLength; }
	inline void SetValue(int inValue) { this->GetCurrentWindowItem().intValue_maxTextValueLength = inValue; }
	inline const char *GetTextValue() const { return this->GetCurrentWindowItem().textValue; }
	inline void SetValue(const char *inValue) { STRNCPY(this->GetCurrentWindowItem().textValue, WINDOW_MAXTEXTVALUESIZE, inValue); }

#ifdef DEBUG_MODE
public:
	void CheckAddIndex(int inIndex, const __FlashStringHelper *infunc);
	void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};

#define CURRWIN(lui)								lui.GetCurrentWindowItem()

#define BEGIN_UI(lui, screen, nb)					LcdUi *plui = &lui;	plui->Setup(&screen, nb);
#define WIN											byte
#define WINDOWCHOICE(first, ...)					plui->AddWindowChoice(first, ##__VA_ARGS__);
#define ADDCHOICE(win, text, ...)					plui->AddChoice(win, text);
#define WINDOWCONFIRM(first, prefix, ...)			plui->AddWindowConfirm(first, prefix, ##__VA_ARGS__)
#define WINDOWINT(first, ...)						plui->AddWindowInt(first, ##__VA_ARGS__)
#define WINDOWINTERRUPT(first, second, ...)			plui->AddWindowInterrupt(first, second, ##__VA_ARGS__)
#define WINDOWSPLASH(first, second, ...)			plui->AddWindowSplash(first, second, ##__VA_ARGS__)
#define WINDOWTEXT(first, ...)						plui->AddWindowText(first, ##__VA_ARGS__)
#define WINDOWYESNO(first, ...)						plui->AddWindowYesNo(first, ##__VA_ARGS__)
#define WINDOWCUSTOM(win, ...)						plui->AddWindowCustom(win, ##__VA_ARGS__)
#define END_UI()
