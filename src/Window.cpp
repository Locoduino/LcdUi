/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "Window.hpp"

#ifdef LCDUI_DEBUG_MODE
void Window::printState(byte inState, const __FlashStringHelper *inFunc)
{
	Serial.print(inFunc);
	Serial.print(": ");
	switch (inState)
	{
	case STATE_NONE:	Serial.println(F("STATE_NONE"));	break;
	case STATE_START:	Serial.println(F("STATE_START"));	break;
	case STATE_INITIALIZE:	Serial.println(F("STATE_INITIALIZE"));	break;
	case STATE_LOOP:	Serial.println(F("STATE_LOOP"));	break;
	case STATE_ABORTED:	Serial.println(F("STATE_ABORTED"));	break;
	case STATE_CONFIRMED:	Serial.println(F("STATE_CONFIRMED"));	break;
	case STATE_POSTCONFIRMED:	Serial.println(F("STATE_POSTCONFIRMED"));		break;
	default:	Serial.println(inState);		break;
	}
}
#endif

Window::Window(byte inFirstLine, int inTag)
{
	this->firstLine = inFirstLine;
	this->state = STATE_START;
	this->tag = inTag;
	this->pNextWindow = NULL;
	this->pFatherWindow = NULL;
	this->fatherChoiceValue = 255;
}

#ifdef LCDUI_PRINT_WINDOWS
void Window::printWindowHeader(const __FlashStringHelper *inName)
{
	Serial.print(inName);
	Serial.print(F(": FirstLine: "));
	Serial.print(this->firstLine);
	if (this->GetFatherWindow() != NULL)
	{
		Serial.print(F(" / Father: "));
		Serial.print(this->GetFatherWindow()->GetFirstLine());
		Serial.print(F(" / FatherChoice: "));
		Serial.print(this->fatherChoiceValue);
	}
}
#endif

