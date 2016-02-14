/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a choice window>
*************************************************************/

#include "LcdUi.h"
#include "WindowChoiceText.hpp"

WindowChoiceText::WindowChoiceText(byte inFirstLine, int inTag) : Window(inFirstLine, inTag)
{
}

void WindowChoiceText::SetCurrentChoice(char *inChoiceText, unsigned int inChoiceValue)
{
	this->currentValue = inChoiceValue;
	this->currentValueText = inChoiceText;
}

void WindowChoiceText::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->GetFirstLine());

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
		pScreen->DisplayChoice(this->currentValueText);
	}
}