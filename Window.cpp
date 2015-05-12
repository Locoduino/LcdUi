/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "Window.hpp"

Window::Window(byte inFirstLine)
{
	this->firstLine = inFirstLine;
	this->state = STATE_START;
	this->choiceValue = 0;
}

