/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl(byte inLevel, int inFirstLine, byte inMoreLessIncrement) : Window(inLevel, inFirstLine)
{
	this->Speed = 0;
	this->DirectionToLeft = false;
	this->MoreLessIncrement = inMoreLessIncrement;
}

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_POSTSTART)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->dccMsg);

		pScreen->setCursor(3, 0);
		pScreen->write(32);
		pScreen->setCursor(4, 0);
		Screen::BuildString(DccId, Screen::buffer, 3);
		pScreen->print(Screen::buffer);
		pScreen->setCursor(7, 0);
		pScreen->write(32);

		byte len = Screen::BuildStringLeft(this->Name, 8, Screen::buffer);
		pScreen->setCursor(pScreen->GetSizeX() - len, 0);
		pScreen->print(Screen::buffer);

		this->state = STATE_POSTSTART;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
	{
		int newValue = this->Speed + this->MoreLessIncrement;
		if (newValue > this->MaxSpeed)
			newValue = this->MaxSpeed;
		this->Speed = newValue;
	}
	showValue = true;
	break;
	case EVENT_LESS:
	{
		int newValue = this->Speed - this->MoreLessIncrement;
		if (newValue < 0)
			newValue = 0;
		this->Speed = newValue;
	}
	showValue = true;
	break;
	case EVENT_MOVE:
		break;
	case EVENT_SELECT:
		this->DirectionToLeft = !this->DirectionToLeft;
		showValue = true;
		break;
	case EVENT_CANCEL:
		this->state = STATE_ABORTED;
		break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 003 nomloco11
		// 1 +>>>>>			 -
		//   01234567879012345
		Screen::BuildProgress(this->Speed, this->MaxSpeed, this->DirectionToLeft, pScreen->GetSizeX(), Screen::buffer);
		pScreen->setCursor(0, 1);
		pScreen->print(Screen::buffer);
	}
}

