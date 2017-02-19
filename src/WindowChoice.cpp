/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a multiple choice screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowChoice.hpp"

void WindowChoice::begin(byte inFirstLine, Choice *inpSelectedChoice)
{
	Window::begin(inFirstLine);

	this->pSelectedChoice = inpSelectedChoice;
}

byte WindowChoice::AddChoice(byte inStringNumber, byte inIndex, byte inInterruptIdOnEscape)
{
	Choice *pChoice = new Choice();
	pChoice->id = inStringNumber;
	if (inIndex == 255)
		inIndex = this->Choices.GetCount();
	pChoice->index = inIndex;
	pChoice->escapeWindowId = inInterruptIdOnEscape;

	this->Choices.AddItem(pChoice);

	if (this->Choices.GetCount() == 1)
		*(this->pSelectedChoice) = *pChoice;

	return inStringNumber;
}

// Get the next choice value from choices, if the current selection is the last one, return to the first...
void WindowChoice::MoveNextChoice()
{
	this->Choices.NextCurrent();
	*(this->pSelectedChoice) = *(this->Choices.pCurrentItem->Obj);
}

// Get the previous choice value from choices, if the current selection is the first one, go to the last...
void WindowChoice::MovePreviousChoice()
{
	this->Choices.PreviousCurrent();
	*(this->pSelectedChoice) = *(this->Choices.pCurrentItem->Obj);
}

void WindowChoice::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->clear();
		pScreen->DisplayText(this->GetFirstLine(), 0, 0);

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
		this->MoveNextChoice();
		showValue = true;
		break;
	case EVENT_LESS:
		this->MovePreviousChoice();
		showValue = true;
		break;
	case EVENT_SELECT:
		this->SetState(STATE_CONFIRMED);
		break;
	case EVENT_CANCEL:
		this->SetState(STATE_ABORTED);
		break;
	}

	if (showValue)
	{
		byte index = 0;

		if (this->Choices.pFirst == NULL)
			return;

		LCDUICHAINEDLISTITEM<Choice> *pCurr = this->Choices.pFirst;

		// Find index of current item.
		while (pCurr != NULL)
		{
			if (pCurr == this->Choices.pCurrentItem)
				break;
			pCurr = pCurr->pNext;
			index++;
		}

		if (index > pScreen->FirstChoiceShown + pScreen->GetSizeY() - 2)
			pScreen->FirstChoiceShown = index - pScreen->GetSizeY() + 2;
		if (index < pScreen->FirstChoiceShown)
			pScreen->FirstChoiceShown = index;

		index = 0;
		pCurr = this->Choices.pFirst;

		while (pCurr != NULL)
		{
			if (index >= pScreen->FirstChoiceShown)
				pScreen->DisplayChoice(pCurr->Obj->id, index, false, pCurr->Obj->id == this->Choices.pCurrentItem->Obj->id);
			pCurr = pCurr->pNext;
			index++;
			if (index > pScreen->FirstChoiceShown + pScreen->GetSizeY() - 2)
				break;
		}
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowChoice::printWindow()
{
	printWindowHeader(F("Window Choice"));
	Serial.println("");

	LCDUICHAINEDLISTITEM<Choice> *pCurr = this->Choices.pFirst;
	while (pCurr != NULL)
	{
		Serial.print("    Choice id:");
		Serial.print(pCurr->Obj->id);
		if (pCurr->Obj->index != 255)
		{
			Serial.print(" / Index:");
			Serial.print(pCurr->Obj->index);
		}
		if (pCurr->Obj->escapeWindowId != 255)
		{
			Serial.print(" / EscapeId:");
			Serial.print(pCurr->Obj->escapeWindowId);
		}
		Serial.println("");
		pCurr = pCurr->pNext;
	}
}
#endif
