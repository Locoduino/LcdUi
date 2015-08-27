/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a 2 lines parallel screen>
*************************************************************/

#include "LCDUI_VStudioMfcDlg.h"

#include "LcdUI.h"
#include "ScreenVS.hpp"

void ScreenVS::Setup(byte sizex, byte sizey)
{
	this->sizex = sizex;
	this->sizey = sizey;
	clearVS();
}

void ScreenVS::clearVS()
{
	CLCDUI_VStudioMfcDlg::pMainDialog->clear();
}

void ScreenVS::printVS(int inPosy, const char *inString)
{
	char buffer[80];

	if (inString == 0)
		CLCDUI_VStudioMfcDlg::pMainDialog->getLine(inPosy, buffer, 80);
	else
		strcpy(buffer, inString);

	if (cursorOn)
	{
		int length = strlen(buffer);
		if (length < this->posx)
			for (int i = length; i < this->posx && i < this->sizex; i++)
				buffer[i] = 32;

		if (this->posx < this->sizex)
			buffer[this->posx] = 2;

		if (this->posx > length)
			buffer[this->posx + 1] = 0;
	}

	buffer[this->sizex] = 0;
	CLCDUI_VStudioMfcDlg::pMainDialog->print(inPosy, buffer);
}

void ScreenVS::delayVS(int inDelay)
{
	Sleep(inDelay);
}

void ScreenVS::print(const char *inString)
{
	char buffer[80];

	CLCDUI_VStudioMfcDlg::pMainDialog->getLine(this->posy, buffer, 80);
	int i;
	for (i = 0; i < (int) strlen(inString); i++)
		buffer[this->posx + i] = inString[i];
	for (; this->posx + i < this->sizex; i++)
		buffer[this->posx + i] = 32;
	buffer[this->sizex] = 0;

	printVS(this->posy, buffer);
}

void ScreenVS::print(int inValue, char inType)
{
	char buffer[80];
	Screen::BuildString(inValue, buffer);

	print(buffer);
}

void ScreenVS::write(char inValue)
{
	char buffer[80];

	CLCDUI_VStudioMfcDlg::pMainDialog->getLine(this->posy, buffer, 80);
	if (this->posx < this->sizex)
		buffer[this->posx] = inValue;
	buffer[this->sizex] = 0;

	printVS(this->posy, buffer);
}

void ScreenVS::clear()
{
	for (int i = 0; i < this->sizey; i++)
		printVS(i, "                              ");
}

void ScreenVS::cursor_on()
{
	cursorOn = true;
	printVS(this->posy, 0);

}

void ScreenVS::cursor_off()
{
	cursorOn = false;
	printVS(this->posy, 0);
}

void ScreenVS::blink_on()
{
	blinkOn = true;
}

void ScreenVS::blink_off()
{
	blinkOn = false;
}



