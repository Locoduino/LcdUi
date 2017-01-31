/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowText.hpp"

void WindowText::begin(byte inFirstLine, char *pValue, byte inMaxLengthValue)
{ 
	Window::begin(inFirstLine);

#ifdef LCDUI_DEBUG_MODE
	if (LcdScreen::BackspaceMsg == -1)
		Serial.println(F("BackspaceMsg undefined !"));
#endif
	this->maxTextValueLength = inMaxLengthValue;
	this->currentCharPos = 0;
	this->currentCharEdited = 0;
	this->pTextValue = pValue;
	memset(this->pTextValue, 0, inMaxLengthValue < WINDOW_MAXTEXTVALUESIZE? inMaxLengthValue: WINDOW_MAXTEXTVALUESIZE);
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
		this->currentCharPos = pScreen->MoveNextChar(this->currentCharPos);
		showValue = true;
		break;

	case EVENT_LESS:
		this->currentCharPos = pScreen->MovePreviousChar(this->currentCharPos);
		showValue = true;
		break;

	case EVENT_SELECT:
		this->state = STATE_SELECTCHAR;
		/*if (this->currentCharPos == 0)
		{
			this->state = STATE_CONFIRMED;
//			pScreen->noCursor();
		}
		else*/
		{
			if (this->currentCharPos == 0) // backspace
			{
				this->currentCharEdited--;
				if (this->currentCharEdited < 0 || this->currentCharEdited == 255)
					this->currentCharEdited = 0;
				this->pTextValue[this->currentCharEdited] = 0;
			}
			else
			{
				this->pTextValue[this->currentCharEdited] = this->currentCharPos;
				this->pTextValue[this->currentCharEdited + 1] = 0;
				this->currentCharEdited++;
			}
			showValue = true;
		}
		break;
	case EVENT_CANCEL:
		this->state = STATE_CONFIRMED;
//		pScreen->noCursor();
		break;
	}

	if (showValue)
	{
		pScreen->clear();
		pScreen->DisplayTextResult(this->pTextValue, this->maxTextValueLength, this->currentCharEdited);
		pScreen->DisplayTextChoice(this->currentCharPos, this->currentCharEdited);
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowText::printWindow()
{
	printWindowHeader(F("Window Text"));
	Serial.print(F(" / maxTextValueLength: "));
	Serial.println(this->maxTextValueLength);
}
#endif