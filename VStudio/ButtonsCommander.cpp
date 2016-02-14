/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Buttons Commander>
*************************************************************/

#ifndef NO_BUTTONSCOMMANDER
#include "arduino.h"
#include "serial.hpp"
#include "ButtonsCommander.hpp"

#include<stdarg.h>

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef DEBUG_MODE
void ButtonsCommander::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
	if (this->size == 0)
	{
		Serial.print(F("Size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->size)
		{
			Serial.print(F("Index error in "));
			Serial.println(inFunc);
		}
}
#endif

ButtonsCommander::ButtonsCommander()
{
	this->size = 0;
	this->pButtons = 0;
	this->addCounter = 0;
}

void ButtonsCommander::Setup(int inSize)
{
#ifdef DEBUG_MODE
	Serial.println(F("   ButtonsCommander Setup"));
#endif
	this->size = inSize;
	this->addCounter = 0;
	this->pButtons = new ButtonsCommanderButton*[inSize];
}

void ButtonsCommander::Setup(int inSize, ButtonsCommanderButton *inpFirstState, ...)
{
#ifdef DEBUG_MODE
	Serial.println(F("   ButtonsCommander Setup"));
#endif
	va_list argList;

	this->size = inSize;
	this->addCounter = 0;
	this->pButtons = new ButtonsCommanderButton*[inSize];
	this->pButtons[this->addCounter++] = inpFirstState;
	inSize--;

	va_start(argList, inpFirstState);
	for (; inSize; inSize--)
	{
		ButtonsCommanderButton *but;
		but = va_arg(argList, ButtonsCommanderButton *);
		this->pButtons[this->addCounter++] = but;
	}
	va_end(argList);
#ifdef DEBUG_MODE
	if (inSize > 0)
		Serial.println(F("   Not enough items in the list !"));
#endif

}

// Returns the index of the new added accessory.
unsigned char ButtonsCommander::Add(ButtonsCommanderButton *inpButton)
{
	CHECK(addCounter, "ButtonsCommander::Add");
	this->pButtons[addCounter++] = inpButton;

	return addCounter - 1;
}

void ButtonsCommander::Set(unsigned char inIndex, ButtonsCommanderButton *inpButton)
{
	CHECK(inIndex, "ButtonsCommander::Set");
	this->pButtons[inIndex] = inpButton;
}

int ButtonsCommander::IndexOf(ButtonsCommanderButton *inpButton)
{
	for (int i = 0; i < this->addCounter; i++)
		if (this->pButtons[i] == inpButton)
			return i;

	return -1;
}

static int loopIndix = 0;

bool ButtonsCommander::Loop()
{
	Commander::StaticData.CommanderPriorityLoop();

	if (loopIndix >= this->addCounter)
		loopIndix = 0;

	CHECK(loopIndix, "ButtonsCommander::Loop()");

	if (!this->pButtons[loopIndix++]->Loop())
		return false;

	this->pButtons[loopIndix-1]->EndLoop();

	return true;
}

#endif
