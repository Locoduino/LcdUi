/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Sample for customized Window>
*************************************************************/

#include "LcdUI.h"
#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl(int inFirstLine) : Window(inFirstLine)
{
}

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
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
		pScreen->DisplayHeader(inFirstLine);
		pScreen->setCursor(3, 0);
		pScreen->write(32);
		pScreen->setCursor(4, 0);
		Screen::BuildString(10, Screen::buffer, 4);
		pScreen->print(Screen::buffer);
		pScreen->setCursor(4 + 4/*digits*/, 0);
		pScreen->write(32);
		byte len = Screen::BuildStringLeft("Sample", pScreen->GetSizeX() - (4 + 4/*digits*/ + 1), Screen::buffer);
		pScreen->setCursor(pScreen->GetSizeX() - len, 0);
		pScreen->print(Screen::buffer);

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
		case EVENT_MORE:
			{
			unsigned int newValue = this->pHandle->Speed + this->pHandle->MoreLessIncrement;
			if (newValue > this->pHandle->GetLocomotive().GetSteps())
				newValue = this->pHandle->GetLocomotive().GetSteps();
			this->pHandle->SetSpeed(newValue);
			}
			showValue = true;
			break;

		case EVENT_LESS:
			{
			int newValue = this->pHandle->Speed - this->pHandle->MoreLessIncrement;
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
			}
			showValue = true;
			break;

		case EVENT_MOVE:
			break;
		case EVENT_SELECT:
			this->pHandle->SetDirection(!this->pHandle->DirectionToLeft);
			showValue = true;
			break;
		case EVENT_CANCEL:
			this->state = STATE_ABORTED;
			break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 0003   sample
		// 1 +>>>>>			 -
		//   01234567879012345
		Screen::BuildProgress(this->pHandle->Speed, this->pHandle->GetLocomotive().GetSteps(), this->pHandle->DirectionToLeft, pScreen->GetSizeX(), Screen::buffer);
		pScreen->setCursor(0, 1);
		pScreen->print(Screen::buffer);
	}
}

