/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a splash screen (time limited info)>
*************************************************************/

#include "LcdUi.h"

WindowSplash::WindowSplash(byte inFirstLine, byte inSecondLine, unsigned long inDelay) : Window(inFirstLine)
{ 
	this->secondLine = inSecondLine;
	this->delay = inDelay;
	this->startingDate = 0;
}

void WindowSplash::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
		return;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->firstLine);
		pScreen->setCursor(0, 1);
		pScreen->GetString(this->secondLine);

		pScreen->print(Screen::buffer);

		this->startingDate = millis();

		this->state = STATE_INITIALIZE;
		return;
	}

	unsigned long val = millis();

	if (millis() - this->startingDate > this->delay)
	{
		this->startingDate = 0;
		this->state = STATE_POSTCONFIRMED;
	}
}

