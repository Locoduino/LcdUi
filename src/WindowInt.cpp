/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowInt.hpp"

WindowInt::WindowInt(byte inFirstLine, int inMaxIntValue, int inMinIntValue, int inTag) : Window(inFirstLine, inTag)
{
	this->maxIntValue = inMaxIntValue;
	this->minIntValue = inMinIntValue;
	this->intValue = 0;
}

void WindowInt::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		inpLcd->GetScreen()->DisplayHeader(this->firstLine);

		if (this->intValue > this->maxIntValue)
			this->intValue = this->maxIntValue;
		if (this->intValue < this->minIntValue)
			this->intValue = this->minIntValue;

		this->state = STATE_INITIALIZE;
	}

	int newValue = this->intValue;
	switch (inEventType)
	{
		case EVENT_MORE:
			newValue++;
			if (newValue <= this->maxIntValue)
				this->intValue = newValue;
			showValue = true;
			break;
		case EVENT_LESS:
			newValue--;
			if (newValue >= this->minIntValue)
				this->intValue = newValue;
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
		inpLcd->GetScreen()->DisplayInt(this->intValue);
	}
}

