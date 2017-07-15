/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"

void WindowYesNo::begin(byte inFirstLine, bool *inpValue)
{
	Window::begin(inFirstLine);

#ifdef LCDUI_DEBUG_MODE
	if (LcdScreen::YesMsg == -1)
		Serial.println(F("YesMsg undefined !"));
	if (inpValue != NULL && LcdScreen::NoMsg == -1)
		Serial.println(F("NoMsg undefined !"));
#endif
	this->pValue = inpValue;
}

void WindowYesNo::Event(byte inEventType, LcdUi *inpLcd)
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
		pScreen->DisplayHeader(this->firstLine);

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
	case EVENT_LESS:
	case EVENT_MOVE:
		if (this->pValue != NULL)
		{
			*(this->pValue) = !*(this->pValue);
			showValue = true;
		}
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
		pScreen->DisplayYesNo(this->pValue == NULL || *(this->pValue)? LcdScreen::YesMsg: LcdScreen::NoMsg, this->pValue == NULL);
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowYesNo::printWindow()
{
	printWindowHeader(F("Window YesNo"));
	Serial.print(F(" / OnlyYes: "));
	Serial.print(this->pValue == NULL ? "Yes" : "No");
	Serial.println("");
}
#endif