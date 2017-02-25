/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a confirm dialog in interrupt context>
*************************************************************/

#include "LcdUi.h"
#include "WindowInterruptConfirm.hpp"

void WindowInterruptConfirm::begin(byte inFirstLine, byte inSecondLine, byte inEventType)
{ 
	WindowInterrupt::begin(inFirstLine, inSecondLine, inEventType);

#ifdef LCDUI_DEBUG_MODE
	if (LcdScreen::YesMsg == -1)
		Serial.println(F("YesMsg undefined !"));
	if (LcdScreen::NoMsg == -1)
		Serial.println(F("NoMsg undefined !"));
#endif
}

void WindowInterruptConfirm::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		this->answer = LcdScreen::NoMsg;
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
		if (this->answer == LcdScreen::YesMsg)
			this->answer = LcdScreen::NoMsg;
		else
			this->answer = LcdScreen::YesMsg;
		showValue = true;
		break;
	case EVENT_SELECT:
#ifdef LCDUI_DEBUG_MODE
		Serial.print(F("Interrup confirmed : "));
		Serial.println(this->answer == LcdScreen::YesMsg ? "Yes" : "No");
#endif
		this->state = STATE_CONFIRMED;
		break;
	case EVENT_CANCEL:
#ifdef LCDUI_DEBUG_MODE
		Serial.println(F("Interrup aborted"));
#endif
		this->state = STATE_ABORTED;
		break;
	}

	if (showValue)
	{
		pScreen->DisplayYesNo(this->answer, this->secondLine);
	}
}


#ifdef LCDUI_PRINT_WINDOWS
void WindowInterruptConfirm::printWindow()
{
	printWindowHeader(F("Window Interrupt Confirm"));
	Serial.print(F(" / SecondLine: "));
	Serial.print(this->secondLine);
	Serial.print(F(" / EventType: "));
	Serial.print(this->eventType);
	Serial.print(F(" / Answer: "));
	Serial.println(this->answer);
}
#endif