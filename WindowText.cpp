/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowText.hpp"

// char table : 32-127

WindowText::WindowText(byte inFirstLine, byte inMaxLengthValue) : Window(inFirstLine)
{ 
	this->maxTextValueLength = inMaxLengthValue;
	this->currentCharPos = 0;
	memset(this->textValue, 0, WINDOW_MAXTEXTVALUESIZE);
	memset(this->memoTextValue, 0, WINDOW_MAXTEXTVALUESIZE);
}

void WindowText::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		this->currentCharEdited = strlen(this->textValue);
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
	case EVENT_MOVE:
		break;
	case EVENT_SELECT:
		this->state = STATE_SELECTCHAR;
		if (this->currentCharPos == 0)
		{
			this->state = STATE_CONFIRMED;
			pScreen->cursor_off();
		}
		else
		{
			if (this->currentCharPos == 1) // backspace
			{
				this->currentCharEdited--;
				if (this->currentCharEdited < 0 || this->currentCharEdited == 255)
					this->currentCharEdited = 0;
				this->textValue[this->currentCharEdited] = 0;
			}
			else
			{
				this->textValue[this->currentCharEdited] = Screen::GetChar(this->currentCharPos);
				this->textValue[this->currentCharEdited + 1] = 0;
				this->currentCharEdited++;
			}
			showValue = true;
		}
		break;
	case EVENT_CANCEL:
		this->state = STATE_ABORTED;
		pScreen->cursor_off();
		break;
	}

	if (showValue)
	{
		pScreen->DisplayTextResult(this->textValue, this->maxTextValueLength, this->currentCharEdited);
		pScreen->DisplayTextChoice(this->currentCharPos, this->currentCharEdited);
	}
}
