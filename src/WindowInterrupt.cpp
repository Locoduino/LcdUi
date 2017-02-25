/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for an interruption window>
*************************************************************/

#include "LcdUi.h"
#include "WindowInterrupt.hpp"

void WindowInterrupt::begin(byte inFirstLine, byte inSecondLine, byte inEventType)
{ 
	Window::begin(inFirstLine);

	this->secondLine = inSecondLine;
	this->eventType = inEventType;
}

void WindowInterrupt::Event(byte inEventType, LcdUi *inpLcd)
{
	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		return;
	}

	if (this->state == STATE_START)
	{
		inpLcd->GetScreen()->DisplayHeader(this->firstLine);
		inpLcd->GetScreen()->DisplayText(this->secondLine, 0, 1);

		this->state = STATE_INITIALIZE;
		return;
	}

	// An interrupt window can only be escaped by cancel key, but we use
	// STATE_CONFIRMED to get the event in the main loop and do what is necessary
	// at window exit.
	if (inEventType == EVENT_CANCEL)
		this->state = STATE_CONFIRMED;
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowInterrupt::printWindow()
{
	printWindowHeader(F("Window Interrupt"));
	Serial.print(F(" / SecondLine: "));
	Serial.print(this->secondLine);
	Serial.print(F(" / EventType: "));
	Serial.println(this->eventType);
}
#endif