/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "LcdUi.h"

#ifndef VISUALSTUDIO
#include "arduino.h"
#include <avr/pgmspace.h>
#endif

bool LcdUi::firstLoop = false;

#ifdef LCDUI_DEBUG_MODE
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
#endif

LcdUi::LcdUi()
{
	this->pScreen = NULL;
	this->pFirstWindow = NULL;
	this->pNodeFather = NULL;
	this->pCurrentWindow = NULL;
	this->pWindowInterrupt = NULL;
}

void FillBuffer(const __FlashStringHelper *str)
{
	const char *p = (const char *)str;
	int k;
	for (k = 0; k < 40 && pgm_read_byte(p + k) != 0x00; k++)
	{
		char myChar = pgm_read_byte_near(p + k);
		LcdScreen::buffer[k] = myChar;
	}
	LcdScreen::buffer[k] = 0;
}

void LcdUi::begin(LcdScreen *inpScreen)
{
#ifdef LCDUI_DEBUG_MODE
	Serial.println(F(""));
	Serial.println(STR_LCDTITLE);
	Serial.println(STR_LCDCOPYRIGHT);
	Serial.println(F(""));

	Serial.println(F("*** Setup LcdUi started."));
#endif

	this->pScreen = inpScreen;

#ifdef LCDUI_DEBUG_MODE
	for (int line = 0; line < this->pScreen->GetSizeY(); line++)
		this->pScreen->DisplayText(F("                    "), 0, line);

	this->pScreen->DisplayText(F("===================="), 0, 0);
	this->pScreen->DisplayText(F("===================="), 0, this->pScreen->GetSizeY() - 1);

	if (this->pScreen->GetSizeX() <= 24)
		FillBuffer(STR_LCDTITLE16);
	else
		FillBuffer(STR_LCDTITLE);
	this->pScreen->DisplayText(LcdScreen::buffer, 0, (this->pScreen->GetSizeY() / 2) - 1);

	if (this->pScreen->GetSizeX() <= 28)
		FillBuffer(STR_LCDCOPYRIGHT16);
	else
		FillBuffer(STR_LCDCOPYRIGHT);
	this->pScreen->DisplayText(LcdScreen::buffer, 0, (this->pScreen->GetSizeY() / 2));

	delay(4000);
#endif
}

Window *LcdUi::AddWindow(Window *inpWindow, Window *inpFatherWindow, byte inChoiceNumber)
{
	if (this->pFirstWindow == NULL)
	{
		this->pFirstWindow = inpWindow;
		return inpWindow;
	}

	Window *pCurr = this->pFirstWindow;

	while (pCurr->GetNextWindow() != NULL)
		pCurr = pCurr->GetNextWindow();

	inpWindow->SetFatherWindow(inpFatherWindow);
	pCurr->SetNextWindow(inpWindow);

	if (this->pCurrentWindow == NULL)
		this->MoveToWindow(inpWindow);

	return inpWindow;
}

Window *LcdUi::AddWindowInterrupt(WindowInterrupt *inpWindow)
{
	//return this->GetWindowIndex(this->AddWindow(inpWindow));
	return NULL;
}

Window *LcdUi::GetNextChildWindow(Window *inRef)
{
	byte actualChoice = inRef->GetFatherChoiceValue();

	Window *pCurr = inRef;

	while (pCurr->GetNextWindow() != NULL)
	{
		if (pCurr->GetFatherWindow() == inRef->GetFatherWindow())
		{
			if (pCurr->GetType() == WINDOWTYPE_INTERRUPT)
				continue;
			byte choice = pCurr->GetFatherChoiceValue();
			if (actualChoice == 255 || choice == actualChoice)
				return pCurr;
		}
		pCurr = pCurr->GetNextWindow();
	}

#ifdef LCDUI_DEBUG_MODE
	Serial.println(F("Window next child not found !"));
#endif
	return NULL;
}

void LcdUi::MoveToNextUIWindow()
{
	Window *pNext = NULL;

	if (this->GetType() == WINDOWTYPE_CHOICE)
	{
		// Move to the next window of the same choice index.
		Window *pCurr = this->pCurrentWindow;
		byte selectedChoice = ((WindowChoice *)pCurr)->GetSelectedChoiceIndex();
		while (pCurr->GetNextWindow() != NULL)
		{
			if (pCurr->GetFatherWindow() == this->pCurrentWindow)
			{
				byte choice = pCurr->GetFatherChoiceValue();
				if (choice == 255 || choice == selectedChoice)
				{
					pNext = pCurr;
					break;
				}
			}
			pCurr = pCurr->GetNextWindow();
		}

#ifdef LCDUI_DEBUG_MODE
		if (pNext == NULL)
			Serial.println(F("Window first child not found !"));
#endif
	}

	if (pNext == NULL)
	{
		byte actualChoice = this->pCurrentWindow->GetFatherChoiceValue();

		if (actualChoice == 255)
		{
			// Simply move to the next window
			Window *pCurr = this->pCurrentWindow;
			while (pCurr->GetNextWindow() != NULL)
			{
				if (pCurr->GetFatherWindow() == this->pCurrentWindow->GetFatherWindow())
				{
					if (pCurr->GetType() == WINDOWTYPE_INTERRUPT)
						continue;
					byte choice = pCurr->GetFatherChoiceValue();
					if (choice == 255 || choice == actualChoice)
					{
						pNext = pCurr;
						break;
					}
				}
				pCurr = pCurr->GetNextWindow();
			}

#ifdef LCDUI_DEBUG_MODE
			if (pNext == NULL)
				Serial.println(F("Window next child not found !"));
#endif
		}
	}

	if (pNext != NULL)
	{
		this->MoveToWindow(pNext);
		this->SetState(STATE_START);
	}
}

Window *LcdUi::GetPreviousWindow(Window *inpCurrent) const
{
	Window *pCurr = this->pFirstWindow;
	while (pCurr->GetNextWindow() != NULL)
	{
		if (pCurr->GetNextWindow() == inpCurrent)
			return pCurr;
		pCurr = pCurr->GetNextWindow();
	}
	return NULL;
}

void LcdUi::MoveToPrevUIWindow()
{
	Window *pPrev = this->pCurrentWindow->GetFatherWindow();
	if (pPrev != NULL)
	{
		WindowChoice *pChoice = (WindowChoice *) pPrev;
		if (pChoice != NULL && this->GetState() == STATE_ABORTED)
		{
			byte escapeWindow = pChoice->GetSelectedChoiceEscapeWindowId();
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

		Window *pCurr = this->GetPreviousWindow(this->pCurrentWindow);
		while (pCurr != NULL)
		{
			if (pCurr->GetFatherWindow() == this->pCurrentWindow->GetFatherWindow())
			{
				if (pCurr->GetType() == WINDOWTYPE_INTERRUPT)
					continue;
				pPrev = pCurr;
				break;
			}
			pCurr = this->GetPreviousWindow(pCurr);
		}

		if (pPrev == NULL)
			this->GetGlobalCurrentWindow()->SetState(STATE_NONE);
	}

	if(pPrev != NULL)
	{
		this->MoveToWindow(pPrev);
		this->SetState(STATE_START);
	}
}

Window *LcdUi::GetWindowById(byte inId) const
{
	Window *pCurr = this->pFirstWindow;
	while (pCurr->GetNextWindow() != NULL)
	{
		if (pCurr->GetWindowId() == inId)
			return pCurr;
		pCurr = pCurr->GetNextWindow();
	}

#ifdef LCDUI_DEBUG_MODE
	Serial.println(F("Window id not found !"));
#endif
	return NULL;
}

bool LcdUi::loop(byte inEvent)
{
#ifdef LCDUI_DEBUG_MODE
	if (firstLoop)
	{
		Serial.println(F("*** Setup LCDui Finished."));
		firstLoop = false;
	}
#endif

	if (inEvent == EVENT_NONE && this->GetState() == STATE_NONE && this->GetType() != WINDOWTYPE_SPLASH)
		return false;

	if (this->pWindowInterrupt != NULL)
	{
		this->pWindowInterrupt->Event(inEvent, this);
		if (this->GetState() == STATE_POSTCONFIRMED)
		{
#ifdef LCDUI_DEBUG_MODE
			Serial.println(F("Interrupted"));
#endif
			if (this->pWindowInterrupt != NULL && this->GetType() == WINDOWTYPE_CONFIRM)
			{
				Window *prev = this->pCurrentWindow->GetFatherWindow();
				this->MoveToWindow(prev);
			}
			InterruptEnd();
		}
		else
			if (this->GetState() == STATE_ABORTED)
			{
				InterruptEnd();
#ifdef LCDUI_DEBUG_MODE
				Serial.println(F("Interrupt aborted"));
#endif
			}
		return true;
	}

	this->pCurrentWindow->Event(inEvent, this);

	if (this->GetState() == STATE_POSTCONFIRMED)
	{
		Window *pCurr = this->pCurrentWindow;
		this->MoveToNextUIWindow();
		if (pCurr == this->pCurrentWindow)
			this->MoveToPrevUIWindow();
	}

	if (this->GetState() == STATE_ABORTED)
		this->MoveToPrevUIWindow();

	if (this->GetState() == STATE_CONFIRMED)
		this->SetState(STATE_POSTCONFIRMED);

	return true;
}

void LcdUi::Interrupt(Window *inpWindow)
{
	this->pWindowInterrupt = inpWindow;
	this->SetState(STATE_START);
	this->pWindowInterrupt->Event(EVENT_NONE, this);
}

void LcdUi::InterruptEnd()
{
	this->pWindowInterrupt = NULL;
	this->SetState(STATE_START);
	this->pCurrentWindow->Event(EVENT_NONE, this);
}

bool LcdUi::IsValueModified(byte inWindowId)
{
	if (this->GetGlobalCurrentWindow()->GetWindowId() == inWindowId)
		if (this->GetState() == STATE_POSTCONFIRMED)
			return true;

	return false;
}
