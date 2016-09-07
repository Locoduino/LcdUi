/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowText.hpp"

// char table : 32-127

WindowText::WindowText(byte inFirstLine, char *pValue, byte inMaxLengthValue, int inTag) : Window(inFirstLine, inTag)
{ 
	this->maxTextValueLength = inMaxLengthValue;
	this->currentCharPos = 0;
	this->currentCharEdited = 0;
	this->pTextValue = pValue;
	memset(this->pTextValue, 0, WINDOW_MAXTEXTVALUESIZE);
}

void WindowText::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		this->currentCharEdited = strlen(this->pTextValue);
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
	{
		int newValue = this->currentCharPos + 1;
		if (newValue < 129)
			this->currentCharPos = newValue;
	}
	showValue = true;
	break;
	case EVENT_LESS:
	{
		int newValue = this->currentCharPos - 1;
		if (newValue >= 0)
			this->currentCharPos = newValue;
	}
	showValue = true;
	break;
	case EVENT_SELECT:
		this->state = STATE_SELECTCHAR;
		if (this->currentCharPos == 0)
		{
			this->state = STATE_CONFIRMED;
//			pScreen->noCursor();
		}
		else
		{
			if (this->currentCharPos == 1) // backspace
			{
				this->currentCharEdited--;
				if (this->currentCharEdited < 0 || this->currentCharEdited == 255)
					this->currentCharEdited = 0;
				this->pTextValue[this->currentCharEdited] = 0;
			}
			else
			{
				this->pTextValue[this->currentCharEdited] = LcdScreen::GetChar(this->currentCharPos);
				this->pTextValue[this->currentCharEdited + 1] = 0;
				this->currentCharEdited++;
			}
			showValue = true;
		}
		break;
	case EVENT_CANCEL:
		this->state = STATE_ABORTED;
//		pScreen->noCursor();
		break;
	}

	if (showValue)
	{
		pScreen->DisplayTextResult(this->pTextValue, this->maxTextValueLength, this->currentCharEdited);
		pScreen->DisplayTextChoice(this->currentCharPos, this->currentCharEdited);
	}
}
