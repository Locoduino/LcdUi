/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a choice window>
*************************************************************/

#include "LcdUi.h"
#include "WindowChoiceText.hpp"

WindowChoiceText::WindowChoiceText(byte inFirstLine, byte *inpValue, int inTag) : Window(inFirstLine, inTag)
{
	this->pValue = inpValue;
}

void WindowChoiceText::SetCurrentChoice(char *inChoiceText, byte inChoiceValue)
{
	this->currentValueText = inChoiceText;
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
		showValue = true;
	}

	if (this->state == STATE_START)
	{
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
		if (*this->pValue > pScreen->FirstChoiceShown + pScreen->GetSizeY() - 2)
			pScreen->FirstChoiceShown++;
		if (*this->pValue < pScreen->FirstChoiceShown)
			pScreen->FirstChoiceShown = 0;
		for (byte i = 0; i < this->GetChoiceTextNumber(); i++)
			pScreen->DisplayChoice(this->GetChoiceTextValue(i), i, i == *this->pValue);
	}
}