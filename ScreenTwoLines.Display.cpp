/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a 2 lines parallel screen>
*************************************************************/

#include "LcdUi.h"

void ScreenTwoLines::DisplayHeader(int inHeader)
{
	this->clear();
	this->setCursor(0, 0);
	this->print(this->GetString(inHeader));
}

void ScreenTwoLines::DisplayChoice(int inCurrentChoice)
{
	this->GetChoiceString(inCurrentChoice);
	DisplayChoice(Screen::buffer);
}

void ScreenTwoLines::DisplayChoice(const char *inChoice)
{
	byte pos = (this->sizex / 2) - (strlen(inChoice) / 2);
	if (strlen(inChoice) > this->sizex)
		pos = 0;

	for (int i = 0; i < pos; i++)
	{
		this->setCursor(i, 1);
		this->write(' ');
	}
	this->setCursor(pos, 1);
	this->print(inChoice);

	this->setCursor(pos + 1 + strlen(inChoice), 1);
	for (int i = pos + 1 + strlen(inChoice) + 1; i < this->sizex; i++)
	{
		this->setCursor(i, 1);
		this->write(' ');
	}
}

void ScreenTwoLines::DisplayInt(int inValue)
{
	this->clearLine(1);

	this->BuildString(inValue, Screen::buffer+1);
	Screen::buffer[0] = '>';
	DisplayChoice(Screen::buffer);
	int len = strlen(buffer);
	buffer[len] = '<';
	buffer[len + 1] = 0;
}

void ScreenTwoLines::DisplayYesNo(byte inChoiceValue, int inPrefixString)
{
	this->clearLine(1);

	byte pos = 0;
	if (inPrefixString != 255)
	{
		this->GetString(inPrefixString);
		this->setCursor(0, 1);
		this->print(Screen::buffer);
		pos = strlen(Screen::buffer);
	}

	if (inChoiceValue == Screen::YesMsg)
		this->GetChoiceString(Screen::YesMsg);
	else
		this->GetString(Screen::YesMsg);

	this->setCursor(pos + 1, 1);
	this->print(Screen::buffer);

	pos = pos + 1 + strlen(Screen::buffer) + 1;

	if (inChoiceValue == Screen::NoMsg)
		this->GetChoiceString(Screen::NoMsg);
	else
		this->GetString(Screen::NoMsg);

	pos++;

	this->setCursor(pos + 1, 1);
	this->print(Screen::buffer);
}

void ScreenTwoLines::DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar)
{
	this->cursor_off();
	this->setCursor(0, 0);
	this->print(inTextValue);
	for (int i = strlen(inTextValue); i < inLength && i < sizex; i++)
	{
		this->setCursor(i, 0);
		this->write('_');
	}
}

void ScreenTwoLines::DisplayTextChoice(byte inPos, byte inEditedChar)
{
	this->cursor_off();
	this->setCursor(0, 1);
	int start = sizex / 2 - 1;

	//   'ABCD>E<FGHIJ'

	this->setCursor(start, 1);
	this->write('>');
	this->setCursor(start+1, 1);
	this->write(GetChar(inPos));
	this->setCursor(start+2, 1);
	this->write('<');

	/*for (int i = 0; i < sizex; i++)
	{
		Screen::buffer[i] = GetChar(start + i);
	} */
//	Screen::buffer[sizex] = 0;
//	this->print(Screen::buffer);
	this->setCursor(inEditedChar, 0);
	this->cursor_on();
}

