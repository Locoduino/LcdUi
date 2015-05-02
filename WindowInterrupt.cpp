/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowInterrupt.hpp"

WindowInterrupt::WindowInterrupt(byte inFirstLine, byte inSecondLine) : Window(inFirstLine)
{ 
	this->secondLine = inSecondLine;
}

void WindowInterrupt::Event(byte inEventType, LcdUi *inpLcd)
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
		pScreen->setCursor(0, 1);
		pScreen->GetString(this->secondLine);

		pScreen->print(Screen::buffer);

		this->state = STATE_INITIALIZE;
	}
}

