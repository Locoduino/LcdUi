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

void LcdUi::printEvent(byte inEvent, const __FlashStringHelper *inFunc)
{
	Serial.print(inFunc);
	Serial.print(": ");
	switch (inEvent)
	{
	case EVENT_NONE:	Serial.println(F("EVENT_NONE"));	break;
	case EVENT_MORE:	Serial.println(F("EVENT_MORE"));	break;
	case EVENT_LESS:	Serial.println(F("EVENT_LESS"));	break;
	case EVENT_SELECT:	Serial.println(F("EVENT_SELECT"));	break;
	case EVENT_CANCEL:	Serial.println(F("EVENT_CANCEL"));	break;
	case EVENT_MOVE:	Serial.println(F("EVENT_MOVE"));	break;
	case EVENT_END:		Serial.println(F("EVENT_END"));		break;
	default:			Serial.println(inEvent);			break;
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
	this->CurrentWindow = 255;
	this->WindowInterrupt = 255;
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
	if (this->windowSize == inNbWindows)
		return;

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
	byte index = this->GetWindowIndex(inpFatherWindow);
	if (index == 255)
		this->pNodeFather[this->windowAddcounter] = 0;
	else
		this->pNodeFather[this->windowAddcounter] = (index + 1) * 100 + (inChoiceNumber == 255 ? 0 : inChoiceNumber + 1);

	if (this->windowAddcounter == 0)
		this->SetWindow(0);

	this->windowAddcounter++;
	return inpWindow;
}

byte LcdUi::GetWindowIndex(Window *inpWindow) const
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (this->pWindows[ref1] == inpWindow)
			return ref1;

#ifdef DEBUG_MODE
	Serial.println(F("Window not found !"));
#endif
	return 255;
}

byte LcdUi::GetChildWindow(byte inRef, byte inChoice)
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (this->GetFather(ref1) == inRef)
		{
			byte choice = this->GetFatherChoice(ref1);
			if (inChoice == 255 || choice == inChoice)
				return ref1;
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window first child not found !"));
#endif
	return 255;
}

byte LcdUi::GetNextChildWindow(byte inRef)
{
	byte actualChoice = this->GetFatherChoice(inRef);

	for (int ref1 = inRef + 1; ref1 < this->windowAddcounter; ref1++)
		if (this->GetFather(ref1) == this->GetFather(inRef))
		{
			if (pWindows[ref1]->GetType() == WINDOWTYPE_INTERRUPT)
				continue;
			byte choice = this->GetFatherChoice(ref1);
			if (actualChoice == 255 || choice == actualChoice)
				return ref1;
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window next child not found !"));
#endif
	return 255;
}

byte LcdUi::GetPrevSiblingWindow(byte inRef)
{
	byte actualChoice = this->GetFatherChoice(inRef);

	for (int ref1 = inRef - 1; ref1 >= 0; ref1--)
		if (this->GetFather(ref1) == this->GetFather(inRef))
		{
			if (pWindows[ref1]->GetType() == WINDOWTYPE_INTERRUPT)
				continue;
			return ref1;
		}

#ifdef DEBUG_MODE
	Serial.println(F("Window previous sibling not found !"));
#endif
	return 255;
}

byte LcdUi::GetParentWindow(byte inRef)
{
	byte father = this->GetFather(inRef);
	if (father != 255)
		return father;

#ifdef DEBUG_MODE
	Serial.println(F("Window parent not found !"));
#endif
	return 255;
}

void LcdUi::GetNextUIWindow()
{
	byte next = 255;

	if (this->GetType() == WINDOWTYPE_CHOICE)
	{
		WindowChoice *pChoice = (WindowChoice *) this->GetGlobalCurrentWindow();
		next = this->GetChildWindow(this->CurrentWindow, pChoice->GetChoiceIndex());
	}
	if (next == 255)
		next = this->GetNextChildWindow(this->CurrentWindow);

	if (next != 255)
	{
		SetWindow(next);
		this->SetState(STATE_START);
	}
}

void LcdUi::GetPrevUIWindow()
{
	byte prev = this->GetParentWindow(this->CurrentWindow);
	if (prev != 255)
	{
		WindowChoice *pChoice = (WindowChoice *) this->pWindows[prev];
		if (pChoice != 0 && this->GetState() == STATE_ABORTED)
		{
			byte escapeWindow = pChoice->GetChoiceEscapeWindow();
			if (escapeWindow != 255)
			{
				this->Interrupt(this->GetWindowById(escapeWindow));
				return;
			}
		}
	}
	else
	{
		// If no parent : look for the previous sibling window...
		prev = this->GetPrevSiblingWindow(this->CurrentWindow);
		if (prev == 255)
			this->SetState(STATE_NONE);
	}

	if(prev != 255)
	{
		this->SetWindow(prev);
		this->SetState(STATE_START);
	}
}

byte LcdUi::GetWindowById(byte inId) const
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (pWindows[ref1]->GetWindowId() == inId)
			return ref1;

#ifdef DEBUG_MODE
	Serial.println(F("Window id not found !"));
#endif
	return 255;
}

bool LcdUi::Loop(byte inEvent)
{
	if (inEvent == EVENT_NONE && this->GetState() == STATE_NONE && this->GetType() != WINDOWTYPE_SPLASH)
		return false;

	if (this->WindowInterrupt != 255)
	{
		this->pWindows[this->WindowInterrupt]->Event(inEvent, this);
		if (this->GetState() == STATE_POSTCONFIRMED)
		{
#ifdef DEBUG_MODE
			Serial.println(F("Interrupted"));
#endif
			if (this->WindowInterrupt != 255 && this->GetType() == WINDOWTYPE_CONFIRM)
			{
				byte prev = this->GetParentWindow(this->CurrentWindow);
				this->SetWindow(prev);
			}
			InterruptEnd();
		}
		else
			if (this->GetState() == STATE_ABORTED)
			{
				InterruptEnd();
#ifdef DEBUG_MODE
				Serial.println(F("Interrupt aborted"));
#endif
			}
		return true;
	}

	this->pWindows[this->CurrentWindow]->Event(inEvent, this);

	if (this->GetState() == STATE_POSTCONFIRMED)
	{
		byte curr = this->CurrentWindow;
		GetNextUIWindow();
		if (curr == this->CurrentWindow)
			GetPrevUIWindow();
	}

	if (this->GetState() == STATE_ABORTED)
		GetPrevUIWindow();

	return true;
}

void LcdUi::Interrupt(byte inWindow)
{
	this->WindowInterrupt = inWindow;
	this->SetState(STATE_START);
	this->pWindows[this->WindowInterrupt]->Event(EVENT_NONE, this);
}

void LcdUi::InterruptEnd()
{
	this->WindowInterrupt = 255;
	this->SetState(STATE_START);
	this->pWindows[this->CurrentWindow]->Event(EVENT_NONE, this);
}
