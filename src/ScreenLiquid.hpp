//-------------------------------------------------------------------
#ifndef __screenLiquid_H__
#define __screenLiquid_H__
//-------------------------------------------------------------------

#include "LcdScreen.hpp"

#ifndef _LCDVS_H_
#include "LiquidCrystal.h"
#endif

//-------------------------------------------------------------------

#define SCREEN	this->pLcd

class ScreenLiquid : public LcdScreen
{
private:
	LiquidCrystal *pLcd;

public:
	inline ScreenLiquid() : LcdScreen() 
	{ 
		this->pLcd = NULL; 
	}

	void clearLine(int posy)
	{
		memset(LcdScreen::buffer, ' ', this->sizex);
		LcdScreen::buffer[this->sizex] = 0;

		this->setCursor(0, posy);
		this->print(LcdScreen::buffer);
	}

	void begin(byte inSizeX, byte inSizeY, PGM_P const *inpStringTable, LiquidCrystal *inpLcd)
	{
		this->pLcd = inpLcd;

		LcdScreen::begin(inSizeX, inSizeY, inpStringTable);

		clear();
	}

	// LCD API
	//LibraryName
#ifndef VISUALSTUDIO
	inline void print(const __FlashStringHelper *inString) { SCREEN->print(inString); }
#endif
	inline void print(const char *inString) { SCREEN->print(inString); }
	inline void print(int inValue, char inType) { SCREEN->print(inValue, inType); }
	inline void write(char inValue) { SCREEN->write(inValue); }
	inline void clear() { SCREEN->clear(); }
	inline void home() { SCREEN->home();	}
	inline void setCursor(byte Col, byte Row) { SCREEN->setCursor(Col, Row); }
	inline void display() { SCREEN->display(); }
	inline void noDisplay() { SCREEN->noDisplay(); }
	inline void cursor() { SCREEN->cursor(); }
	inline void noCursor() { SCREEN->noCursor(); }
	inline void blink() { SCREEN->blink(); }
	inline void noBlink() { SCREEN->noBlink(); }

	/////////////

	void DisplayChar(char car, byte inX, byte inY)
	{
		this->setCursor(inX, inY);
		this->write(car);
	}

	void DisplayText(char *inText, byte inX, byte inY)
	{
		this->setCursor(inX, inY);
		this->print(inText);
	}

	void DisplayHeader(int inHeader)
	{
		this->clear();
		this->DisplayText(this->GetString(inHeader), 0, this->HeaderY);
	}

	void DisplayChoice(int inCurrentChoice, int inIndex, bool inShowIndex, bool inChoosen)
	{
		this->GetString(inCurrentChoice);

		if (inShowIndex)
		{
			// postfix with a number...
			char index[10];
			this->BuildString(inIndex + 1, index);
			int len = strlen(LcdScreen::buffer);
			for (unsigned int i = 0; i <= strlen(index); i++)
				LcdScreen::buffer[len + i] = index[i];
		}
		DisplayChoice(LcdScreen::buffer, inIndex, inChoosen);
	}

	void DisplayChoice(const char *inChoice, int inIndex, bool inChoosen)
	{
		if (inIndex < this->FirstChoiceShown)
			return;

		if (inIndex > this->FirstChoiceShown + this->sizey - 2)
			return;

		int line = inIndex - this->FirstChoiceShown + 1;

		byte pos = (this->sizex / 2) - (strlen(inChoice) / 2) - 1;
		for (int i = 0; i < pos; i++)
			this->DisplayChar(' ', i, line);

		this->DisplayChar(inChoosen ? '>' : ' ', pos++, line);

		this->DisplayText((char *)inChoice, pos, line);
		pos += strlen(inChoice);

		if (inChoosen)
			this->DisplayChar('<', pos++, line);

		for (int i = pos; i < this->sizex; i++)
			this->DisplayChar(' ', i, line);
	}

	void DisplayInt(int inValue)
	{
		this->clearLine(this->SecondLineY);

		this->BuildString(inValue, LcdScreen::buffer);
		byte pos = (this->sizex / 2) - (strlen(LcdScreen::buffer) / 2) - 1;

		this->DisplayChar('>', pos, this->SecondLineY);
		this->DisplayText(LcdScreen::buffer, pos + 1, this->SecondLineY);
		this->DisplayChar('<', pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);
	}

	void DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar)
	{
		this->noCursor();
		this->DisplayText((char *)inTextValue, 0, this->HeaderY);

		for (int i = strlen(inTextValue); i < inLength && i < sizex; i++)
			this->DisplayChar('_', i, this->HeaderY);
	}

	void DisplayTextChoice(byte inPos, byte inEditedChar)
	{
		this->noCursor();
		this->setCursor(0, this->SecondLineY);
		int start = sizex / 2 - 1;

		//   'ABCD>E<FGHIJ'

		this->DisplayChar('>', start, this->SecondLineY);
		this->DisplayChar(GetChar(inPos), start + 1, this->SecondLineY);
		this->DisplayChar('<', start + 2, this->SecondLineY);

		/*for (int i = 0; i < sizex; i++)
		{
		LcdScreen::buffer[i] = GetChar(start + i);
		} */
		//	LcdScreen::buffer[sizex] = 0;
		//	this->print(LcdScreen::buffer);
		this->setCursor(inEditedChar, this->HeaderY);
		this->cursor();
	}

	void DisplayYesNo(byte inChoiceValue, int inPrefixString = 0)
	{
		this->clearLine(this->SecondLineY);

		byte pos = 0;
		if (inPrefixString != 0)
		{
			this->GetString(inPrefixString);
			this->DisplayText(LcdScreen::buffer, 0, this->SecondLineY);
			pos = strlen(LcdScreen::buffer);
		}

		if (inChoiceValue != LcdScreen::NoMsg)
			this->DisplayChar('>', pos, this->SecondLineY);

		this->GetString(LcdScreen::YesMsg);
		this->DisplayText(LcdScreen::buffer, pos + 1, this->SecondLineY);

		if (inChoiceValue != LcdScreen::NoMsg)
			this->DisplayChar('<', pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);

		pos = pos + 1 + strlen(LcdScreen::buffer) + 1;

		this->GetString(LcdScreen::NoMsg);

		pos++;
		if (inChoiceValue == LcdScreen::NoMsg)
			this->DisplayChar('>', pos, this->SecondLineY);

		this->DisplayText(LcdScreen::buffer, pos + 1, this->SecondLineY);

		if (inChoiceValue == LcdScreen::NoMsg)
			this->DisplayChar('<', pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);
	}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
