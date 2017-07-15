/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a choice window>
*************************************************************/

#include "LcdUi.h"
#include "WindowChoiceText.hpp"

void WindowChoiceText::begin(byte inFirstLine, byte *inpValue)
{
	Window::begin(inFirstLine);
	this->pValue = inpValue;
}

void WindowChoiceText::SetCurrentChoice(byte inChoiceValue)
{
	*this->pValue = inChoiceValue;
}

void WindowChoiceText::Move(bool inMore)
{
	unsigned int val = *this->pValue;
	if (inMore)
	{
		val++;
		if (val >= this->GetChoiceTextNumber())
			val = 0;
	}
	else
	{
		if (val == 0)
			val = this->GetChoiceTextNumber() - 1;
		else
			val--;
	}
	SetCurrentChoice(val);
}

void WindowChoiceText::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		if (pScreen->FirstChoiceShown >= this->GetChoiceTextNumber())
			pScreen->FirstChoiceShown = 0;
		if (*this->pValue >= this->GetChoiceTextNumber())
			*this->pValue = 0;
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

	if (showValue && !this->lockScreen)
	{
		if (*this->pValue > pScreen->FirstChoiceShown + pScreen->GetSizeY() - 2)
			pScreen->FirstChoiceShown = *this->pValue - pScreen->GetSizeY() + 2;
		if (*this->pValue < pScreen->FirstChoiceShown)
			pScreen->FirstChoiceShown = *this->pValue;

		for (byte i = 0; i < this->GetChoiceTextNumber(); i++)
			pScreen->DisplayChoice(this->GetChoiceTextValue(i, pScreen), i, i == *this->pValue);
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowChoiceText::printWindow()
{
	printWindowHeader(F("Window Choice Text"));
	Serial.println("");
}
#endif
