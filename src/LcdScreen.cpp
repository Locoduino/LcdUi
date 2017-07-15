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

void LcdScreen::BuildString(long int inValue, char *outString, int digits)
{
	long int pows_of_ten[6] = { 1, 10, 100, 1000, 10000, 100000 };
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

// build a string with '>' or '<' character representing a value inside 0 and a maximum,
// bounded by '+' and '-' according to the given direction:
// '-            +' means 0
// '->>>>>>		 +' means half value
// '->>>>>>>>>>>>+' for full value in From left dir.
// '+<<<<<<<<<<<<-' for full value in From right dir.
////////// An accuracy can be accepted to avoid edge values, for instance 49.99 instead of 50%
// this is not the case of limit values. 0 cannot be reah if the value is not exactly 0,
// And the same for maximum value.
void LcdScreen::BuildProgress(byte inValue, byte inMax, bool inFromLeftDir, byte inLengthString, char *outString)
{
	double exactValue = ((int)(inLengthString - 2) * (((double)inValue) / inMax));
	byte localValue = (byte)(exactValue + 0.5);
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

byte LcdScreen::BuildString(const char *inString, byte inSizeMax, char *outString)
{
	memcpy(outString, inString, inSizeMax);
	outString[inSizeMax] = 0;

	int len = (int)strlen(outString);
	if (len > inSizeMax)
		len = inSizeMax;

	return len;
}

void LcdScreen::FillBuffer(const __FlashStringHelper *str)
{
#ifdef VISUALSTUDIO
	strcpy_s(buffer, BUFFER_SIZE, str);
#else
	const char *p = (const char *)str;
	int k;
	for (k = 0; k < 40 && pgm_read_byte(p + k) != 0x00; k++)
	{
		char myChar = pgm_read_byte_near(p + k);
		LcdScreen::buffer[k] = myChar;
	}
	LcdScreen::buffer[k] = 0;
#endif
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
	int len = (int) strlen(buffer);
	buffer[len] = '<';
	buffer[len+1] = 0;

	return buffer;
}

// ASCII char table : 32-127
//			 A-Z	 a-z	  0-9	 misc chars
// del(0) / 65-90 / 97-122 / 48-57 / 32-47

char LcdScreen::MoveNextChar(char inCurrPos)
{
	int newValue = -1;

	if (inCurrPos == 47)	return 47;
	if (inCurrPos == 0)		newValue = 65;
	if (inCurrPos == 90)	newValue = 97;
	if (inCurrPos == 122)	newValue = 48;
	if (inCurrPos == 57)	newValue = 32;

	if (newValue == -1)
		newValue = inCurrPos + 1;

	return newValue;
}

char LcdScreen::MovePreviousChar(char incurrPos)
{
	int newValue = -1;
	if (incurrPos == 0)	return 0;
	if (incurrPos == 32)	newValue = 57;
	if (incurrPos == 48)	newValue = 122;
	if (incurrPos == 97)	newValue = 90;
	if (incurrPos == 65)	newValue = 0;

	if (newValue == -1)
		newValue = incurrPos - 1;

	return newValue;
}

