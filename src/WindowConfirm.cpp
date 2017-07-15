/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowConfirm.hpp"

void WindowConfirm::begin(byte inFirstLine, byte inPrefix, bool *inpValue)
{ 
	Window::begin(inFirstLine);

#ifdef LCDUI_DEBUG_MODE
	if (LcdScreen::YesMsg == -1)
		Serial.println(F("YesMsg undefined !"));
	if (LcdScreen::NoMsg == -1)
		Serial.println(F("NoMsg undefined !"));
#endif

	this->prefix = inPrefix;
	this->pValue = inpValue;
}

void WindowConfirm::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		if (this->pValue != NULL)
			*(this->pValue) = false;
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
		if(this->pValue != NULL)
			*(this->pValue) = !*(this->pValue);
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
		pScreen->DisplayYesNo(this->pValue == NULL || *(this->pValue) ? LcdScreen::YesMsg : LcdScreen::NoMsg, this->pValue == NULL, this->prefix);
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowConfirm::printWindow()
{
	printWindowHeader(F("Window Confirm"));
	Serial.print(F(" / Prefix: "));
	Serial.print(this->prefix);
	Serial.print(F(" / OnlyYes: "));
	Serial.print(this->pValue == NULL ? "Yes":"No");
	Serial.println("");
}
#endif
