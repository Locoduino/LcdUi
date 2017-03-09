/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco control window>
*************************************************************/

#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl() : Window(-1)
{
	this->isDcc = true;
	this->dcFrequency = 0;
	this->dcFrequencyText = NULL;
	this->Address = 3;
	this->AddressSize = 3;
	this->Speed = 0;
	this->SpeedMax = 128;
	this->Speed128Inc = 10;
	this->Direction = true;
	strcpy(this->Name, "Locoduino");
}

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	LcdScreen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->clear();
		if (this->isDcc)
		{
			pScreen->DisplayText("Dcc ", 0, 0);
			LcdScreen::BuildString(this->Address, LcdScreen::buffer, this->AddressSize);
			pScreen->DisplayText(LcdScreen::buffer, 4, 0);
			byte len = LcdScreen::BuildStringLeft(this->Name, inpLcd->GetScreen()->GetSizeX() - (2 + 1), LcdScreen::buffer);
			pScreen->DisplayText(LcdScreen::buffer, pScreen->GetSizeX() - len, 0);
		}
		else
		{
			pScreen->DisplayText("Dc", 0, 0);
			if (this->dcFrequencyText != NULL)
			{
				byte len = LcdScreen::BuildStringLeft(this->dcFrequencyText, inpLcd->GetScreen()->GetSizeX() - (2 + 1), LcdScreen::buffer);
				pScreen->DisplayText(LcdScreen::buffer, pScreen->GetSizeX() - len, 0);
			}
		}
		this->state = STATE_INITIALIZE;
	}

	byte inc = 1;

	if (this->SpeedMax == 128)
		inc = this->Speed128Inc;
 
	switch (inEventType)
	{
		case EVENT_MORE:
			{
#ifdef DDC_DEBUG_MODE
			Serial.print(F("MORE "));
#endif
			unsigned int newValue = this->Speed + inc;
			if (newValue > this->SpeedMax)
				newValue = this->SpeedMax;
			this->Speed = newValue;
#ifdef DDC_DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

		case EVENT_LESS:
			{
#ifdef DDC_DEBUG_MODE
			Serial.print(F("LESS "));
#endif
			int newValue = this->Speed - inc;
			if (newValue < 0)
				newValue = 0;
			this->Speed = newValue;
#ifdef DDC_DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

		case EVENT_SELECT:
#ifdef DDC_DEBUG_MODE
			Serial.println(F("SELECT"));
#endif
			this->Direction = !this->Direction;
			showValue = true;
			break;
		case EVENT_CANCEL:
#ifdef DDC_DEBUG_MODE
			Serial.println(F("CANCEL"));
#endif
			this->state = STATE_ABORTED;
			break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 003
		// 1 +>>>>>			 -
		//   01234567879012345
		int speed = this->Speed;
		if (speed == 1)
			speed = 0;
		LcdScreen::BuildProgress(speed, this->SpeedMax, this->Direction, pScreen->GetSizeX(), LcdScreen::buffer);
		pScreen->DisplayText(LcdScreen::buffer, 0, 1);
	}
}

