/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowChoice.hpp"

WindowChoice::WindowChoice(byte inFirstLine, int inNumberOfChoices, bool inEscapeWindows, int inTag) : Window(inFirstLine, inTag)
{
	this->choiceAddCounter = 0;
	this->pChoices = new byte[inNumberOfChoices];
	if (inEscapeWindows)
		this->pEscapeWindows = new byte[inNumberOfChoices];
	else
		this->pEscapeWindows = 0;
}

byte WindowChoice::AddChoice(byte inStringNumber, byte inInterruptOnEscape)
{
	if (this->choiceAddCounter < WINDOW_MAXCHOICES)
	{
		this->pChoices[this->choiceAddCounter] = inStringNumber;
		if (pEscapeWindows != 0)
			this->pEscapeWindows[this->choiceAddCounter] = inInterruptOnEscape;
		this->choiceAddCounter++;

		// Set the initial value to the first choice.
		if (this->choiceAddCounter == 1)
			this->choiceValue = inStringNumber;

		return this->choiceAddCounter - 1;
	}

	return 255;
}

byte WindowChoice::GetChoiceIndex() const
{
	for (byte i = 0; i < this->choiceAddCounter; i++)
		if (pChoices[i] == this->choiceValue)
			return i;

	return 255;
}

// Get the next choice value from choices, if the current selection is the last one, return to the first...
void WindowChoice::MoveNextChoice()
{
	byte ind = GetChoiceIndex();

	if (ind == 255)
		this->choiceValue = pChoices[0];

	ind++;
	if (ind >= this->choiceAddCounter)
		ind = 0;
	this->choiceValue = this->pChoices[ind];
}

// Get the previous choice value from choices, if the current selection is the first one, go to the last...
void WindowChoice::MovePreviousChoice()
{
	byte ind = GetChoiceIndex();

	if (ind == 255)
		this->choiceValue = pChoices[0];

	if (ind == 0)
		ind = this->choiceAddCounter;
	this->choiceValue = this->pChoices[--ind];
}

void WindowChoice::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->GetFirstLine());

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
	case EVENT_MORE:
		this->MoveNextChoice();
		showValue = true;
		break;
	case EVENT_LESS:
		this->MovePreviousChoice();
		showValue = true;
		break;
	case EVENT_SELECT:
		this->SetState(STATE_CONFIRMED);
		break;
	case EVENT_CANCEL:
		this->SetState(STATE_ABORTED);
		break;
	}

	if (showValue)
	{
		pScreen->DisplayChoice(this->choiceValue);
	}
}