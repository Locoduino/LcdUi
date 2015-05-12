/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowConfirm.hpp"

WindowConfirm::WindowConfirm(byte inFirstLine, byte inPrefix) : Window(inFirstLine)
{ 
	this->prefix = inPrefix;
}

void WindowConfirm::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		this->choiceValue = Screen::NoMsg;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->firstLine);

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
	case EVENT_LESS:
	case EVENT_MOVE:
		if (this->choiceValue == Screen::YesMsg)
			this->choiceValue = Screen::NoMsg;
		else
			this->choiceValue = Screen::YesMsg;
		showValue = true;
		break;
	case EVENT_SELECT:
		this->state = STATE_CONFIRMED;
		break;
	case EVENT_CANCEL:
		this->state = STATE_ABORTED;
		break;
	}

	if (showValue)
	{
		pScreen->DisplayYesNo(this->choiceValue, this->prefix);
	}
}

