/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowInterrupt.hpp"

WindowInterrupt::WindowInterrupt(byte inFirstLine, byte inSecondLine, int inTag) : Window(inFirstLine, inTag)
{ 
	this->secondLine = inSecondLine;
}

void WindowInterrupt::Event(byte inEventType, LcdUi *inpLcd)
{
	if (this->state == STATE_INITIALIZE)
		this->state = STATE_NONE;

	if (this->state == STATE_START)
	{
		inpLcd->GetScreen()->DisplayHeader(this->firstLine);
		inpLcd->GetScreen()->setCursor(0, 1);
		inpLcd->GetScreen()->GetString(this->secondLine);

		inpLcd->GetScreen()->print(Screen::buffer);

		this->state = STATE_INITIALIZE;
	}
}

