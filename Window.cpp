/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUI.h"
#include "Window.hpp"

#ifdef DEBUG_MODE
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
	this->choiceValue = 0;
	this->tag = inTag;
}

