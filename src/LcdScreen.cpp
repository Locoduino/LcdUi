/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"

char LcdScreen::buffer[];
int LcdScreen::YesMsg = -1;
int LcdScreen::NoMsg = -1;
int LcdScreen::BackspaceMsg = -1;

void LcdScreen::begin(byte inSizeX, byte inSizeY, const char * const *inpStringTable)
{
	this->sizex = inSizeX;
	this->sizey = inSizeY;
	this->pStringTable = inpStringTable;
	this->FirstChoiceShown = 0;
	this->HeaderY = 0;
	this->SecondLineY = 1;
}

void LcdScreen::BuildString(int inValue, char *outString, int digits)
{
	int pows_of_ten[6] = { 1, 10, 100, 1000, 10000 };
	int nb = digits;
	if (digits > 0 && digits < 6)
	{
		inValue = inValue%pows_of_ten[digits];
		for (; nb > 0; nb--)
		{
			if (inValue < pows_of_ten[nb - 1])
			{
				outString[digits - nb] = '0';
				continue;
			}
			outString[digits - nb] = (char)(48 + inValue / pows_of_ten[nb - 1]);
			inValue = inValue % pows_of_ten[nb - 1];
		}
		outString[digits] = 0;
	}
	else
	{
		if (inValue == 0)
		{
			outString[0] = '0';
			outString[1] = 0;
			return;
		}

		signed int started = -1;
		for (nb = 5; nb > 0; nb--)
		{
			if (started < 0 && inValue >= pows_of_ten[nb - 1])
				started = nb;
			if (started >= 0)
			{
				outString[started - nb] = (char)(48 + (inValue / pows_of_ten[nb - 1]));
				inValue = inValue % pows_of_ten[nb - 1];
			}
		}
		outString[started] = 0;
	}
}

void LcdScreen::BuildProgress(byte inValue, byte inMax, bool inFromLeftDir, byte inLengthString, char *outString)
{
	byte localValue = (byte) ((int)(inLengthString-2) * (((double)inValue) / inMax));
	if (localValue == 0 && inValue > 0)
		localValue = 1;

	if (localValue == inLengthString - 2 && inValue < inMax)
		localValue--;

	memset(outString, 32, inLengthString - 1);
	outString[inLengthString] = 0;

	if (inFromLeftDir)
	{
		outString[0] = '-';
		outString[inLengthString - 1] = '+';
		memset(outString+1, '>', localValue);
	}
	else
	{
		outString[0] = '+';
		outString[inLengthString - 1] = '-';
		memset(outString + inLengthString - 1 - localValue, '<', localValue);
	}
}

byte LcdScreen::BuildStringLeft(const char *inString, byte inSizeMax, char *outString)
{
	memcpy(outString, inString, inSizeMax);
	outString[inSizeMax] = 0;

	int len = strlen(outString);
	if (len > inSizeMax)
		len = inSizeMax;

	return len;
}

char *LcdScreen::GetString(int inString)
{
#ifdef VISUALSTUDIO
	strcpy_s(buffer, BUFFER_SIZE, this->pStringTable[inString]);
#else
	strcpy_P(buffer, (char*)pgm_read_word(&this->pStringTable[inString]));
#endif

	return buffer;
}

char *LcdScreen::GetChoiceString(int inString)
{
	buffer[0] = '>';
#ifdef VISUALSTUDIO
	strcpy_s(buffer + 1, BUFFER_SIZE-1, this->pStringTable[inString]);
#else
	strcpy_P(buffer+1, (char*)pgm_read_word(&this->pStringTable[inString]));
#endif
	int len = strlen(buffer);
	buffer[len] = '<';
	buffer[len+1] = 0;

	return buffer;
}

byte LcdScreen::GetChar(int inPos)
{
	if (inPos < 0 || inPos > 127)
		return ' ';

	if (inPos == 0)
		return 127;	// backspace

	return 32 + (inPos - 1);
}

