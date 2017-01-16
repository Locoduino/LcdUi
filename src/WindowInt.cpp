/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowInt.hpp"

WindowInt::WindowInt(byte inFirstLine, int *inpValue, int inMinIntValue, int inMaxIntValue, int inTag) : Window(inFirstLine, inTag)
{
#ifdef LCDUI_DEBUG_MODE
	if (inMinIntValue >= inMaxIntValue)
		Serial.println(F("Integer minimum value greater or equal to maximum !"));
#endif

	this->minIntValue = inMinIntValue;
	this->maxIntValue = inMaxIntValue;
	this->pValue = inpValue;
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

		if (*this->pValue < this->minIntValue)
			*this->pValue = this->minIntValue;
		if (*this->pValue > this->maxIntValue)
			*this->pValue = this->maxIntValue;

		this->state = STATE_INITIALIZE;
	}

	int newValue = *this->pValue;
	switch (inEventType)
	{
		case EVENT_MORE:
			newValue++;
			if (newValue <= this->maxIntValue)
				*this->pValue = newValue;
			showValue = true;
			break;
		case EVENT_LESS:
			newValue--;
			if (newValue >= this->minIntValue)
				*this->pValue = newValue;
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
		inpLcd->GetScreen()->DisplayInt(*this->pValue);
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowInt::printWindow()
{
	printWindowHeader(F("Window Int"));
	Serial.print(F(" / minIntValue: "));
	Serial.print(this->minIntValue);
	Serial.print(F(" / maxIntValue: "));
	Serial.print(this->maxIntValue);
	Serial.println("");
}
#endif