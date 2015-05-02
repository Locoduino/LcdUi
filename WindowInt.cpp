/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowInt.hpp"

WindowInt::WindowInt(byte inFirstLine, int inMaxIntValue, int inMinIntValue) : Window(inFirstLine)
{
	this->maxIntValue = inMaxIntValue;
	this->minIntValue = inMinIntValue;
	this->intValue = 0;
}

void WindowInt::Event(byte inEventType, LcdUi *inpLcd)
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
		pScreen->DisplayHeader(this->firstLine);

		if (this->intValue > this->maxIntValue)
			this->intValue = this->maxIntValue;
		if (this->intValue < this->minIntValue)
			this->intValue = this->minIntValue;

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
	{
		int newValue = this->intValue + 1;
		if (newValue <= this->maxIntValue)
			this->intValue = newValue;
	}
	showValue = true;
	break;
	case EVENT_LESS:
	{
		int newValue = this->intValue - 1;
		if (newValue >= this->minIntValue)
			this->intValue = newValue;
	}
	showValue = true;
	break;
	case EVENT_MOVE:
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
		pScreen->DisplayInt(this->intValue);
	}
}

