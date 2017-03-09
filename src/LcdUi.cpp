/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "LcdUi.h"
#include "../VStudio/LcdUi.hpp"

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

	this->pScreen->clear();

	if (this->pScreen->GetSizeY() > 3)
	{
		this->pScreen->DisplayTextF(F("===================="), 0, 0);
		this->pScreen->DisplayTextF(F("===================="), 0, this->pScreen->GetSizeY() - 1);
	}

	if (this->pScreen->GetSizeX() <= 24)
		LcdScreen::FillBuffer(STR_LCDTITLE16);
	else
		LcdScreen::FillBuffer(STR_LCDTITLE);
	this->pScreen->DisplayText(LcdScreen::buffer, 0, (this->pScreen->GetSizeY() / 2) - 1);

	if (this->pScreen->GetSizeX() <= 28)
		LcdScreen::FillBuffer(STR_LCDCOPYRIGHT16);
	else
		LcdScreen::FillBuffer(STR_LCDCOPYRIGHT);
	this->pScreen->DisplayText(LcdScreen::buffer, 0, (this->pScreen->GetSizeY() / 2));

	delay(2000);
#endif

}

Window *LcdUi::AddWindow(Window *inpWindow)
{
	if (this->pFirstWindow == NULL)
		this->pFirstWindow = inpWindow;
	else
	{
		Window *pCurr = this->pFirstWindow;

		while (pCurr->GetNextWindow() != NULL)
			pCurr = pCurr->GetNextWindow();

		pCurr->SetNextWindow(inpWindow);
	}

	if (this->pCurrentWindow == NULL && inpWindow->GetType() != WINDOWTYPE_INTERRUPT && inpWindow->GetType() != WINDOWTYPE_INTERRUPTCONFIRM)
		this->MoveToWindow(inpWindow);

	return inpWindow;
}

Window *LcdUi::GetNextWindowWithFather(Window *inpStart, Window *inpFatherTofind, byte inChoiceToFind)
{
	Window *pCurr = inpStart->GetNextWindow();

	while (pCurr != NULL)
	{
		if (pCurr->GetType() != WINDOWTYPE_INTERRUPT && pCurr->GetType() != WINDOWTYPE_INTERRUPTCONFIRM)
			if (pCurr->IsActive())
				if (pCurr->GetFatherWindow() == inpFatherTofind)
					if (inChoiceToFind == 255 || inChoiceToFind == pCurr->GetFatherChoiceValue())
						return pCurr;
		pCurr = pCurr->GetNextWindow();
	}

	return NULL;
}

void LcdUi::MoveToNextUIWindow()
{
	Window *pNext = NULL;

	if (this->GetType() == WINDOWTYPE_CHOICE)
	{
		// Move to the next window according to the choice made.
		byte selectedChoice = ((WindowChoice *)this->pCurrentWindow)->GetSelectedChoiceId();
		pNext = this->GetNextWindowWithFather(this->pCurrentWindow, this->pCurrentWindow, selectedChoice);
	}

	if (pNext == NULL)
	{
		byte actualChoice = this->pCurrentWindow->GetFatherChoiceValue();

		// Look for the next window with the same level
		pNext = this->GetNextWindowWithFather(this->pCurrentWindow, this->pCurrentWindow->GetFatherWindow(), actualChoice);

		// if not found
		if (pNext == NULL)
		{
			// if not choice window
			if (actualChoice == 255)
			{
				// If no father, return to this window
				if (this->pCurrentWindow->GetFatherWindow() != NULL)
					pNext = this->pCurrentWindow->GetFatherWindow();
				else
					// Otherwise go to the previous window of the same level.
					pNext = this->GetPreviousWindow(this->pCurrentWindow);
			}
			else
				// if choice window without next window, return to the father.
				pNext = this->pCurrentWindow->GetFatherWindow();
		}
	}

#ifdef LCDUI_DEBUG_MODE
	if (pNext == NULL)
		Serial.println(F("Window next child not found !"));
#endif

	if (pNext != NULL)
	{
		this->MoveToWindow(pNext);
		this->SetState(STATE_START);
	}
}

Window *LcdUi::GetPreviousWindow(Window *inpCurrent) const
{
	Window *pCurr = this->pFirstWindow->GetNextWindow();
	while (pCurr != NULL)
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
			if (pCurr->IsActive())
				if (pCurr->GetFatherWindow() == this->pCurrentWindow->GetFatherWindow())
				{
					if (pCurr->GetType() == WINDOWTYPE_INTERRUPT || pCurr->GetType() == WINDOWTYPE_INTERRUPTCONFIRM)
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
	while (pCurr != NULL)
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

	if (this->GetState() == STATE_CONFIRMED)
		this->SetState(STATE_POSTCONFIRMED);

	if (this->pWindowInterrupt != NULL)
	{
		this->pWindowInterrupt->Event(inEvent, this);

		switch (this->GetState())
		{
		case STATE_POSTCONFIRMED:
		case STATE_ABORTED:
			InterruptEnd();
			break;
		}
		return true;
	}

	if (this->InterruptByEvent(inEvent) != 255)
		return true;

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

	return true;
}

byte LcdUi::InterruptByEvent(byte inEventType)
{
	if (inEventType == 255)
		return 255;

	Window *pCurr = this->pFirstWindow;
	while (pCurr != NULL)
	{
		if (pCurr->IsActive())
			if (pCurr->GetType() == WINDOWTYPE_INTERRUPT || pCurr->GetType() == WINDOWTYPE_INTERRUPTCONFIRM)
				if (((WindowInterrupt *)pCurr)->GetEventType() == inEventType)
				{
					Interrupt(pCurr);
					return pCurr->GetWindowId();
				}
		pCurr = pCurr->GetNextWindow();
	}

	return 255;
}

void LcdUi::Interrupt(byte inWindowId)
{
	Window *pWin = this->GetWindowById(inWindowId);
	if (pWin != NULL)
		Interrupt(pWin);
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
	if (this->pCurrentWindow != NULL)
	{
		this->SetState(STATE_START);
		this->pCurrentWindow->Event(EVENT_NONE, this);
	}
}

bool LcdUi::IsValueModified(byte inWindowId)
{
	if (this->GetGlobalCurrentWindow()->GetWindowId() == inWindowId)
		if (this->GetState() == STATE_POSTCONFIRMED)
			return true;

	return false;
}

#ifdef LCDUI_PRINT_WINDOWS
void LcdUi::printWindows()
{
	Serial.println(F("********* Windows List"));
	Window *pCurr = this->pFirstWindow;

	while (pCurr != NULL)
	{
		pCurr->printWindow();
		pCurr = pCurr->GetNextWindow();
	}
	Serial.println(F("********* End of List"));
}
#endif
