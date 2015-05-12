/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a 2 lines parallel screen>
*************************************************************/

#include "LcdUi.h"

#ifdef VISUALSTUDIO
#include "Serial.hpp"
#endif

#include "ScreenTwoLines.hpp"

#ifdef VISUALSTUDIO
#define SCREEN	this->pScreenVS->
#else
#define SCREEN	this->pLcd->
#endif

#ifndef VISUALSTUDIO
void ScreenTwoLines::print(const __FlashStringHelper *inString)
{
	this->pLcd->print(inString);
}
#endif

void ScreenTwoLines::print(int inValue, char inType)
{
#ifndef VISUALSTUDIO
	SCREEN print(inValue, inType);
#endif
}

void ScreenTwoLines::cursor_on() 
{
#ifdef VISUALSTUDIO
	this->pScreenVS->cursor_on();
#else
	this->pLcd->cursor();
#endif
}

void ScreenTwoLines::cursor_off()
{
#ifdef VISUALSTUDIO
	this->pScreenVS->cursor_off();
#else
	this->pLcd->noCursor();
#endif
}

void ScreenTwoLines::blink_on()
{ 
#ifdef VISUALSTUDIO
	this->pScreenVS->blink_on();
#else
	this->pLcd->blink();
#endif
}

void ScreenTwoLines::blink_off() 
{ 
#ifdef VISUALSTUDIO
	this->pScreenVS->blink_off();
#else
	this->pLcd->noBlink();
#endif
}

void ScreenTwoLines::Setup(byte inSizeX, byte inSizeY,
	PGM_P const *inpStringTable, uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	if (rw == -1)
		this->pLcd = new LiquidCrystalFast(rs, enable, d0, d1, d2, d3);
	else
		this->pLcd = new LiquidCrystalFast(rs, rw, enable, d0, d1, d2, d3);

	Screen::Setup(inSizeX, inSizeY, inpStringTable);

	this->pLcd->begin(inSizeX, inSizeY);   // initialize the lcd for 16 chars 2 lines, turn on back light
	clear();
}
