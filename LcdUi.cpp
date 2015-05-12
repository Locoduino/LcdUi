/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "LcdUi.h"

#ifndef VISUALSTUDIO
#include "arduino.h"
#include <avr/pgmspace.h>
#endif

static int freemem = 0;

#ifdef DEBUG_MODE
void static CheckPinNb(int inPin, const __FlashStringHelper *inFunc)
{
	if (inPin <= 0 || inPin >= NUM_DIGITAL_PINS)
	{
		Serial.print(F("Pin "));
		Serial.print(inPin);
		Serial.print(F(" error in "));
		Serial.println(inFunc);
	}
}

void LcdUi::CheckAddIndex(int inIndex, const __FlashStringHelper *inFunc)
{
	if (this->windowSize == 0)
	{
		Serial.print(F("Windows size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex >= this->windowSize)
		{
			Serial.print(F("Windows size too small in "));
			Serial.println(inFunc);
		}
}

void LcdUi::CheckIndex(int inIndex, const __FlashStringHelper *inFunc)
{
	if (this->windowSize == 0)
	{
		Serial.print(F("Windows size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->windowSize)
		{
			Serial.print(F("Window index error in "));
			Serial.println(inFunc);
		}
}

#define CHECK(val, text)	CheckIndex(val, F(text))
#define CHECKADD(val, text)	CheckAddIndex(val, F(text))
#else
#define CHECK(val, text)
#define CHECKADD(val, text)
#endif

LcdUi::LcdUi()
{
	this->pScreen = 0;
	this->pWindows = 0;
	this->pNodeFather = 0;
	this->windowSize = 0;
	this->windowAddcounter = 0;
	this->pCurrentWindow = 0;
	this->pWindowInterrupt = 0;
}

void FillBuffer(const __FlashStringHelper *str)
{
#ifdef VISUALSTUDIO
	strcpy_s(Screen::buffer, 40, str);
#else
	const char *p = (const char *)str;
	int k;
	for (k = 0; k < pgm_read_byte(p + k) != 0x00; k++)
	{
		char myChar = pgm_read_byte_near(p + k);
		Screen::buffer[k] = myChar;
	}
	Screen::buffer[k] = 0;
#endif
}

void LcdUi::Setup(Screen *inpScreen, int inNbWindows)
{
	this->pScreen = inpScreen;
	this->SetWindowsNumber(inNbWindows);

#ifdef DEBUG_MODE
	this->pScreen->setCursor(0, 0);
	if (this->pScreen->GetSizeX() <= 16)
		FillBuffer(STR_LCDTITLE16);
	else
		FillBuffer(STR_LCDTITLE);

	this->pScreen->print(Screen::buffer);

	this->pScreen->setCursor(0, 1);
	if (this->pScreen->GetSizeX() <= 16)
		FillBuffer(STR_LCDCOPYRIGHT16);
	else
		FillBuffer(STR_LCDCOPYRIGHT);

	this->pScreen->print(Screen::buffer);

#if VISUALSTUDIO
	this->pScreen->pScreenVS->delayVS(2000);
#else
	delay(1000);
#endif
#endif
}

void LcdUi::StartSetup()
{
#ifdef DEBUG_MODE
	Serial.begin(115200);
#ifndef VISUALSTUDIO
	//while (!Serial);
#endif

	// Just for let the time to the PIC to initialize internals...
	delay(500);

	Serial.println(F(""));
	Serial.println(STR_LCDTITLE);
	Serial.println(STR_LCDCOPYRIGHT);
	Serial.println(F(""));

	Serial.println(F("*** Setup LcdUI started."));
#endif
}

void LcdUi::EndSetup()
{	
#ifdef DEBUG_MODE
	Serial.println(F("*** Setup LcdUI Finished."));
#endif
}

void LcdUi::SetWindowsNumber(int inNbWindows)
{
	this->windowSize = inNbWindows;
	this->windowAddcounter = 0;
	if (inNbWindows > 0)
	{
		this->pWindows = new Window *[inNbWindows];
		this->pNodeFather = new int[inNbWindows];
	}
}

Window *LcdUi::AddWindow(Window *inpWindow, Window *inpFatherWindow, byte inChoiceNumber)
{
	CHECKADD(this->windowAddcounter, "AddWindow");
	this->pWindows[this->windowAddcounter] = inpWindow;
	if (inpFatherWindow == 0)
		this->pNodeFather[this->windowAddcounter] = 0;
	else
		this->pNodeFather[this->windowAddcounter] = (GetWindowIndex(inpFatherWindow) + 1) * 100 + (inChoiceNumber == 255?0:inChoiceNumber+1);

	if (this->windowAddcounter == 0)
		this->SetWindow(inpWindow);

	this->windowAddcounter++;

	return inpWindow;
}

int LcdUi::GetWindowIndex(Window *inpRef)
{
	int ref = 0;
	for (ref = 0; ref < this->windowAddcounter; ref++)
		if (this->pWindows[ref] == inpRef)
			return ref;

#ifdef DEBUG_MODE
	Serial.println(F("Window not found !"));
#endif

	return -1;
}

Window *LcdUi::GetChildWindow(Window *inpRef, byte inChoice)
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (this->GetFather(ref1) == inpRef)
		{
			byte choice = this->GetFatherChoice(ref1);
			if (inChoice == 255 || choice == inChoice)
				return pWindows[ref1];
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window first child not found !"));
#endif
	return 0;
}

Window *LcdUi::GetNextChildWindow(Window *inpRef)
{
	int actualChild = GetWindowIndex(inpRef);
	byte actualChoice = this->GetFatherChoice(actualChild);

	for (int ref1 = actualChild + 1; ref1 < this->windowAddcounter; ref1++)
		if (this->GetFather(pWindows[ref1]) == this->GetFather(inpRef))
		{
			if (pWindows[ref1]->GetType() == WINDOWTYPE_INTERRUPT)
				continue;
			byte choice = this->GetFatherChoice(pWindows[ref1]);
			if (actualChoice == 255 || choice == actualChoice)
				return pWindows[ref1];
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window next child not found !"));
#endif
	return 0;
}

Window *LcdUi::GetPrevSiblingWindow(Window *inpRef)
{
	int actualChild = GetWindowIndex(inpRef);
	if (actualChild == 0)
		return 0;

	byte actualChoice = this->GetFatherChoice(actualChild);

	for (int ref1 = actualChild - 1; ref1 >= 0; ref1--)
		if (this->GetFather(pWindows[ref1]) == this->GetFather(inpRef))
		{
			if (pWindows[ref1]->GetType() == WINDOWTYPE_INTERRUPT)
				continue;
			return pWindows[ref1];
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window previous sibling not found !"));
#endif
	return 0;
}

Window *LcdUi::GetParentWindow(Window *inpRef)
{
	int ref = GetWindowIndex(inpRef);

	if (GetFather(ref) != 0)
		return GetFather(ref);

#ifdef DEBUG_MODE
	Serial.println(F("Window parent not found !"));
#endif
	return 0;
}

void LcdUi::GetNextUIWindow()
{
	Window *pNext = 0;

	if (this->pCurrentWindow->GetType() == WINDOWTYPE_CHOICE)
	{
		WindowChoice *pChoice = (WindowChoice *) this->pCurrentWindow;
		pNext = this->GetChildWindow(this->pCurrentWindow, pChoice->GetChoiceIndex());
	}
	if (pNext == 0)
		pNext = this->GetNextChildWindow(this->pCurrentWindow);

	if (pNext != 0)
	{
		SetWindow(pNext);
		pNext->SetState(STATE_START);
	}
}

void LcdUi::GetPrevUIWindow()
{
	Window *pPrev = this->GetParentWindow(this->pCurrentWindow);
	if (pPrev != 0)
	{
		WindowChoice *pChoice = (WindowChoice *) pPrev;
		if (pChoice != 0 && this->pCurrentWindow->GetState() == STATE_ABORTED)
		{
			byte escapeWindow = pChoice->GetChoiceEscapeWindow();
			if (escapeWindow != 255)
			{
				Interrupt((WindowInterrupt *) this->GetWindow(escapeWindow));
				return;
			}
		}
	}
	else
	{
		// If no parent : look for the previous sibling window...
		pPrev = this->GetPrevSiblingWindow(this->pCurrentWindow);
		if (pPrev == 0)
			this->pCurrentWindow->SetState(STATE_NONE);
	}

	if(pPrev != 0)
	{
		SetWindow(pPrev);
		pPrev->SetState(STATE_START);
	}
}

Window *LcdUi::GetWindow(byte inId)
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (pWindows[ref1]->GetWindowId() == inId)
			return pWindows[ref1];

	return 0;
}

bool LcdUi::Loop(byte inEvent)
{
	if (inEvent == EVENT_NONE && this->GetState() == STATE_NONE && this->GetCurrentWindow()->GetType() != WINDOWTYPE_SPLASH)
		return false;

	if (this->pWindowInterrupt != 0)
	{
		this->pWindowInterrupt->Event(inEvent, this);
		if (this->pWindowInterrupt->GetState() == STATE_POSTCONFIRMED)
		{
			if (this->pWindowInterrupt != 0 && this->pWindowInterrupt->GetType() == WINDOWTYPE_CONFIRM)
			{
				Window *pPrev = this->GetParentWindow(this->pCurrentWindow);
				this->SetWindow(pPrev);
			}
			InterruptEnd();
		}
		else
			if (this->pWindowInterrupt->GetState() == STATE_ABORTED)
				InterruptEnd();
		return true;
	}

	this->pCurrentWindow->Event(inEvent, this);

	if (this->pCurrentWindow->GetState() == STATE_POSTCONFIRMED)
	{
		Window *pCurr = this->pCurrentWindow;
		GetNextUIWindow();
		if (pCurr == this->pCurrentWindow)
			GetPrevUIWindow();
	}

	if (this->pCurrentWindow->GetState() == STATE_ABORTED)
		GetPrevUIWindow();

	return true;
}

void LcdUi::Interrupt(WindowInterrupt *inpWindow)
{
	this->pWindowInterrupt = inpWindow;
	this->pWindowInterrupt->SetState(STATE_START);
	this->pWindowInterrupt->Event(EVENT_NONE, this);
}

void LcdUi::InterruptEnd()
{
	this->pWindowInterrupt = 0;
	this->pCurrentWindow->SetState(STATE_START);
	this->pCurrentWindow->Event(EVENT_NONE, this);
}
