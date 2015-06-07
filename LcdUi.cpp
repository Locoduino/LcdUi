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

#define CURR	this->pWindows[this->WindowInterrupt == 255 ? this->CurrentWindow : this->WindowInterrupt]

LcdUi::LcdUi()
{
	this->pScreen = 0;
	this->pWindows = 0;
	this->pNodeFather = 0;
	this->windowSize = 0;
	this->windowAddcounter = 0;
	this->CurrentWindow = 0;
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
	this->windowSize = inNbWindows;
	this->windowAddcounter = 0;
	if (inNbWindows > 0)
	{
		this->pWindows = new WindowItem[inNbWindows];
		this->pNodeFather = new int[inNbWindows];
	}
}

byte LcdUi::AddWindow(byte inType, byte inFirst, byte inSecond, int inMax, int inMin, byte inMaxLength,
	unsigned long inDelay, byte inFatherWindow, byte inChoiceNumber)
{
	CHECKADD(this->windowAddcounter, "AddWindow");
	this->pWindows[this->windowAddcounter].type = inType;
	this->pWindows[this->windowAddcounter].first = inFirst;
	this->pWindows[this->windowAddcounter].second = inSecond;
	this->pWindows[this->windowAddcounter].state = STATE_NONE;
		this->pWindows[this->windowAddcounter].delay_maxIntValue = 0;

	this->pWindows[this->windowAddcounter].choiceValue_currentCharPos = 0;
	this->pWindows[this->windowAddcounter].choiceAddCounter_currentCharEdited = 0;
	this->pWindows[this->windowAddcounter].intValue_maxTextValueLength = 0;
	this->pWindows[this->windowAddcounter].startingDate_minIntValue = 0;
	this->pWindows[this->windowAddcounter].pCustom = 0;

	if (inFatherWindow == 255)
		this->pNodeFather[this->windowAddcounter] = 0;
	else
		this->pNodeFather[this->windowAddcounter] = (inFatherWindow + 1) * 100 + (inChoiceNumber == 255 ? 0 : inChoiceNumber + 1);

	if (this->windowAddcounter == 0)
		this->SetWindow(0);

	this->pWindows[this->windowAddcounter].choices = 0;
	this->pWindows[this->windowAddcounter].escapeWindows = 0;
	this->pWindows[this->windowAddcounter].textValue = 0;

	switch (inType)
	{
	case WINDOWTYPE_SPLASH:
		this->pWindows[this->windowAddcounter].delay_maxIntValue = inDelay;
		break;
	case WINDOWTYPE_INT:
		this->pWindows[this->windowAddcounter].delay_maxIntValue = inMax;
		this->pWindows[this->windowAddcounter].startingDate_minIntValue = inMin;
		break;
	case WINDOWTYPE_TEXT:
		this->pWindows[this->windowAddcounter].textValue = new char[WINDOW_MAXTEXTVALUESIZE + 1];
		this->pWindows[this->windowAddcounter].intValue_maxTextValueLength = inMaxLength;
		break;
	case WINDOWTYPE_CHOICE:
		this->pWindows[this->windowAddcounter].choices = new byte[WINDOW_MAXCHOICES];
		this->pWindows[this->windowAddcounter].escapeWindows = new byte[WINDOW_MAXCHOICES];
		break;
	}

	return this->windowAddcounter++;
}

byte LcdUi::AddWindowCustom(Window *inpCustom, byte inFatherWindow, byte inChoiceNumber)
{
	CHECKADD(this->windowAddcounter, "AddWindowCustom");
	this->pWindows[this->windowAddcounter].pCustom = inpCustom;

	if (inFatherWindow == 255)
		this->pNodeFather[this->windowAddcounter] = 0;
	else
		this->pNodeFather[this->windowAddcounter] = (inFatherWindow + 1) * 100 + (inChoiceNumber == 255 ? 0 : inChoiceNumber + 1);

	if (this->windowAddcounter == 0)
		this->SetWindow(0);

	return this->windowAddcounter++;
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
			if (this->pWindows[ref1].type == WINDOWTYPE_INTERRUPT)
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
			if (this->pWindows[ref1].type == WINDOWTYPE_INTERRUPT)
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
	if (GetFather(inRef) != 255)
		return GetFather(inRef);

#ifdef DEBUG_MODE
	Serial.println(F("Window parent not found !"));
#endif
	return 255;
}

void LcdUi::GetNextUIWindow()
{
	byte next = 255;

	if (this->GetType() == WINDOWTYPE_CHOICE)
		next = this->GetChildWindow(this->CurrentWindow, this->GetChoiceIndex());

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
		if (this->pWindows[prev].type == WINDOWTYPE_CHOICE && this->GetState() == STATE_ABORTED)
		{
			byte escapeWindow = this->pWindows[prev].escapeWindows[this->GetChoiceIndex()];
			if (escapeWindow != 255)
			{
				Interrupt(escapeWindow);
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

	if (prev != 255)
	{
		SetWindow(prev);
		this->SetState(STATE_START);
	}
}

byte LcdUi::GetWindow(byte inId)
{
	for (int ref1 = 0; ref1 < this->windowAddcounter; ref1++)
		if (this->pWindows[ref1].first == inId)
			return ref1;

	return 255;
}

bool LcdUi::Loop(byte inEvent)
{
	if (inEvent == EVENT_NONE && this->GetState() == STATE_NONE && this->GetType() != WINDOWTYPE_SPLASH)
		return false;

	Event(inEvent);

	if (this->WindowInterrupt != 255)
	{
		if (this->GetState() == STATE_POSTCONFIRMED)
		{
			if (this->GetType() == WINDOWTYPE_CONFIRM)
			{
				byte prev = this->GetParentWindow(this->CurrentWindow);
				this->SetWindow(prev);
			}
			InterruptEnd();
		}
		else
			if (this->GetState() == STATE_ABORTED)
				InterruptEnd();
		return true;
	}

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
	this->Event(EVENT_NONE);
}

void LcdUi::InterruptEnd()
{
	this->WindowInterrupt = 255;
	this->SetState(STATE_START);
	this->Event(EVENT_NONE);
}

void LcdUi::AddChoice(byte inOwner, byte inStringNumber, byte inInterruptOnEscape)
{
	if (this->pWindows[inOwner].choiceAddCounter_currentCharEdited < WINDOW_MAXCHOICES)
	{
		this->pWindows[inOwner].choices[this->pWindows[inOwner].choiceAddCounter_currentCharEdited] = inStringNumber;
		this->pWindows[inOwner].escapeWindows[this->pWindows[inOwner].choiceAddCounter_currentCharEdited] = inInterruptOnEscape;
		this->pWindows[inOwner].choiceAddCounter_currentCharEdited++;

		// Set the initial value to the first choice.
		if (this->pWindows[inOwner].choiceAddCounter_currentCharEdited == 1)
			this->pWindows[inOwner].choiceValue_currentCharPos = inStringNumber;
	}
}

byte LcdUi::GetChoiceIndex() const
{
	for (byte i = 0; i < CURR.choiceAddCounter_currentCharEdited; i++)
		if (CURR.choices[i] == CURR.choiceValue_currentCharPos)
			return i;

	return 255;
}

// Get the next choice value from choices, if the current selection is the last one, return to the first...
void LcdUi::MoveNextChoice()
{
	byte ind = GetChoiceIndex();
	WindowItem &curr = CURR;

	if (ind == 255)
		curr.choiceValue_currentCharPos = curr.choices[0];

	ind++;
	if (ind >= curr.choiceAddCounter_currentCharEdited)
		ind = 0;
	curr.choiceValue_currentCharPos = curr.choices[ind];
}

// Get the previous choice value from choices, if the current selection is the first one, go to the last...
void LcdUi::MovePreviousChoice()
{
	WindowItem &curr = CURR;
	byte ind = this->GetChoiceIndex();

	if (ind == 255)
		curr.choiceValue_currentCharPos = curr.choices[0];

	if (ind == 0)
		ind = curr.choiceAddCounter_currentCharEdited;
	curr.choiceValue_currentCharPos = curr.choices[--ind];
}

void LcdUi::Event(byte inEvent)
{
	WindowItem &curr = CURR;
	bool showValue = false;
	if (curr.state == STATE_INITIALIZE)
	{
		curr.state = STATE_NONE;
		switch (curr.type)
		{
		case WINDOWTYPE_YESNO:			curr.choiceValue_currentCharPos = Screen::NoMsg;	break;
			case WINDOWTYPE_INT:
				if (curr.intValue_maxTextValueLength > (int)curr.delay_maxIntValue)
					curr.intValue_maxTextValueLength = (int)curr.delay_maxIntValue;
				if (curr.intValue_maxTextValueLength < (int)curr.startingDate_minIntValue)
					curr.intValue_maxTextValueLength = (int)curr.startingDate_minIntValue;
				break;
		}
		showValue = true;
	}

	if (curr.state == STATE_START)
	{
		this->pScreen->DisplayHeader(curr.first);

		if (curr.second != 255)
		{
			this->pScreen->setCursor(0, 1);
			this->pScreen->GetString(curr.second);
			this->pScreen->print(Screen::buffer);
		}

		switch (curr.type)
		{
		case WINDOWTYPE_CHOICE:		curr.choiceValue_currentCharPos = curr.choices[0];	break;
		case WINDOWTYPE_INT:		curr.intValue_maxTextValueLength = 0;					break;
		case WINDOWTYPE_SPLASH:		curr.startingDate_minIntValue = millis();		break;
		}
		curr.state = STATE_INITIALIZE;
	}

	if (curr.type == WINDOWTYPE_SPLASH)
	{
		unsigned long val = millis();

		if (millis() - curr.startingDate_minIntValue > curr.delay_maxIntValue)
		{
			curr.startingDate_minIntValue = 0;
			curr.state = STATE_POSTCONFIRMED;
		}
		return;
	}

	int newValue = curr.intValue_maxTextValueLength;

	switch (inEvent)
	{
	case EVENT_MORE:
		showValue = true;
		switch (curr.type)
		{
			case WINDOWTYPE_CHOICE:		this->MoveNextChoice();	break;
			case WINDOWTYPE_YESNO:
			case WINDOWTYPE_CONFIRM:	curr.choiceValue_currentCharPos = curr.choiceValue_currentCharPos == Screen::YesMsg ? Screen::NoMsg : Screen::YesMsg;	break;
			case WINDOWTYPE_INT:		newValue++;	if (newValue <= (int)curr.delay_maxIntValue)	curr.intValue_maxTextValueLength = (unsigned long)newValue;	break;
		}
		break;
	case EVENT_LESS:
		showValue = true;
		switch (curr.type)
		{
			case WINDOWTYPE_CHOICE:		this->MovePreviousChoice();	break;
			case WINDOWTYPE_YESNO:
			case WINDOWTYPE_CONFIRM:	curr.choiceValue_currentCharPos = curr.choiceValue_currentCharPos == Screen::YesMsg ? Screen::NoMsg : Screen::YesMsg;	break;
			case WINDOWTYPE_INT:		newValue--;	if (newValue >= (int)curr.startingDate_minIntValue)	curr.intValue_maxTextValueLength = (unsigned long)newValue; break;
		}
		break;
	case EVENT_SELECT:
		curr.state = STATE_CONFIRMED;
		break;
	case EVENT_CANCEL:
		curr.state = STATE_ABORTED;
		break;
	}

	if (showValue)
	{
		switch (curr.type)
		{
		case WINDOWTYPE_CHOICE:		this->pScreen->DisplayChoice(curr.choiceValue_currentCharPos);	break;
			case WINDOWTYPE_YESNO:
			case WINDOWTYPE_CONFIRM:	this->pScreen->DisplayYesNo(curr.choiceValue_currentCharPos, curr.second);	break;
			case WINDOWTYPE_INT:		this->pScreen->DisplayInt(curr.intValue_maxTextValueLength);
		}		
	}
}
