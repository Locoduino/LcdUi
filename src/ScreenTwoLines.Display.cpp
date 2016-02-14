/*************************************************************
project: <LcdUi>
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

void ScreenTwoLines::DisplayChoice(int inCurrentChoice, int inIndex)
{
	this->GetString(inCurrentChoice);
	if (inIndex != -1)
	{
		char index[10];
		this->BuildString(inIndex, index);
		int len = strlen(Screen::buffer);
		for (int i = 0; i <= strlen(index); i++)
			Screen::buffer[len + i] = index[i];
	}
	DisplayChoice(Screen::buffer);
}

void ScreenTwoLines::DisplayChoice(const char *inChoice)
{
	//this->clearLine(1);

	byte pos = (this->sizex / 2) - (strlen(inChoice) / 2) - 1;
	for (int i = 0; i < pos; i++)
	{
		this->setCursor(i, 1);
		this->write(' ');
	}
	this->setCursor(pos, 1);
	this->write('>');

	this->setCursor(pos + 1, 1);
	this->print(inChoice);

	this->setCursor(pos + 1 + strlen(inChoice), 1);
	this->write('<');
	for (int i = pos + 1 + strlen(inChoice) + 1; i < this->sizex; i++)
	{
		this->setCursor(i, 1);
		this->write(' ');
	}
}

void ScreenTwoLines::DisplayInt(int inValue)
{
	this->clearLine(1);

	this->BuildString(inValue, Screen::buffer);
	byte pos = (this->sizex / 2) - (strlen(Screen::buffer) / 2) - 1;
	this->setCursor(pos, 1);
	this->write('>');

	this->setCursor(pos + 1, 1);
	this->print(Screen::buffer);

	this->setCursor(pos + 1 + strlen(Screen::buffer), 1);
	this->write('<');
}

void ScreenTwoLines::DisplayYesNo(byte inChoiceValue, int inPrefixString)
{
	this->clearLine(1);

	byte pos = 0;
	if (inPrefixString != 0)
	{
		this->GetString(inPrefixString);
		this->setCursor(0, 1);
		this->print(Screen::buffer);
		pos = strlen(Screen::buffer);
	}

	this->GetString(Screen::YesMsg);
	if (inChoiceValue != Screen::NoMsg)
	{
		this->setCursor(pos, 1);
		this->write('>');
	}

	this->setCursor(pos + 1, 1);
	this->print(Screen::buffer);

	if (inChoiceValue != Screen::NoMsg)
	{
		this->setCursor(pos + 1 + strlen(Screen::buffer), 1);
		this->write('<');
	}

	pos = pos + 1 + strlen(Screen::buffer) + 1;

	this->GetString(Screen::NoMsg);

	pos++;
	if (inChoiceValue == Screen::NoMsg)
	{
		this->setCursor(pos, 1);
		this->write('>');
	}

	this->setCursor(pos + 1, 1);
	this->print(Screen::buffer);

	if (inChoiceValue == Screen::NoMsg)
	{
		this->setCursor(pos + 1 + strlen(Screen::buffer), 1);
		this->write('<');
	}
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

