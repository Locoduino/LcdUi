/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a confirm dialog in interrupt context>
*************************************************************/

#include "LcdUi.h"
#include "WindowInterruptConfirm.hpp"

WindowInterruptConfirm::WindowInterruptConfirm(byte inFirstLine, byte inSecondLine, int inTag) : WindowInterrupt(inFirstLine, inSecondLine, inTag)
{ 
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
		this->state = STATE_CONFIRMED;
		break;
	case EVENT_CANCEL:
		this->state = STATE_ABORTED;
		break;
	}

	if (showValue)
	{
		pScreen->DisplayYesNo(this->answer, this->secondLine);
	}
}

