/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Keyboard button.>
*************************************************************/

#include "arduino.h"
#include "serial.hpp"
#include "ButtonsCommanderKeyboard.hpp"

#include "conio.h"

char ButtonsCommanderKeyboard::lastLoopKey;

ButtonsCommanderKeyboard::ButtonsCommanderKeyboard() : ButtonsCommanderButton()
{
	this->key = 0;
}

bool ButtonsCommanderKeyboard::Loop()
{
	this->UnselectLastLoop();
	if (this->key == 0)
		return false;

	if (lastLoopKey == this->key)
	{
		this->SelectLastLoop();
		lastLoopKey = 0;
#ifdef DEBUG_MODE
		Serial.print(F("Keyboard "));
		char str[3];
		str[0] = key;
		str[1] = 0;
		Serial.print(str);
		Serial.println(F(" pressed "));
#endif
		return true;
	}

	return false;
}

void ButtonsCommanderKeyboard::EndLoop()
{
}
