//-------------------------------------------------------------------
#ifndef __screenLiquidNew_H__
#define __screenLiquidNew_H__
//-------------------------------------------------------------------

#include "LcdScreen.hpp"

#ifndef _LCDVS_H_
#include "LiquidCrystal.h"
#endif

//-------------------------------------------------------------------

#define SCREEN	this->pLcd

class ScreenLiquidNew : public LcdScreen
{
private:
	LCD *pLcd;

public:
	ScreenLiquidNew() : LcdScreen() 
	{ 
		this->pLcd = 0; 
	}

	void clearLine(int posy)
	{
		for (unsigned int i = 0; i <= this->sizex; i++)
		{
			this->setCursor(i, posy);
			this->write(' ');
		}
	}

	void begin(byte inSizeX, byte inSizeY, PGM_P const *inpStringTable, LCD *inpLcd)
	{
		this->pLcd = inpLcd;

		LcdScreen::begin(inSizeX, inSizeY, inpStringTable);

		this->pLcd->begin(inSizeX, inSizeY);   // initialize the lcd for X chars Y lines
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

	void DisplayHeader(int inHeader)
	{
		this->clear();
		this->setCursor(0, this->HeaderY);
		this->print(this->GetString(inHeader));
	}
	
	void DisplayText(char *inText, byte inX, byte inY)
	{
		this->setCursor(inX, inY);
		this->print(inText);
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
		{
			this->setCursor(i, line);
			this->write(' ');
		}

		this->setCursor(pos++, line);
		if (inChoosen)
			this->write('>');
		else
			this->write(' ');

		this->setCursor(pos, line);
		this->print(inChoice);
		pos += strlen(inChoice);

		if (inChoosen)
		{
			this->setCursor(pos++, line);
			this->write('<');
		}

		for (int i = pos; i < this->sizex; i++)
		{
			this->setCursor(i, line);
			this->write(' ');
		}
	}
	
	void DisplayInt(int inValue)
	{
		this->clearLine(this->SecondLineY);

		this->BuildString(inValue, LcdScreen::buffer);
		byte pos = (this->sizex / 2) - (strlen(LcdScreen::buffer) / 2) - 1;
		this->setCursor(pos, this->SecondLineY);
		this->write('>');

		this->setCursor(pos + 1, this->SecondLineY);
		this->print(LcdScreen::buffer);

		this->setCursor(pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);
		this->write('<');
	}
	
	void DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar)
	{
		this->noCursor();
		this->setCursor(0, this->HeaderY);
		this->print(inTextValue);
		for (int i = strlen(inTextValue); i < inLength && i < sizex; i++)
		{
			this->setCursor(i, this->HeaderY);
			this->write('_');
		}
	}

	void DisplayTextChoice(byte inPos, byte inEditedChar)
	{
		this->noCursor();
		this->setCursor(0, this->SecondLineY);
		int start = sizex / 2 - 1;

		//   'ABCD>E<FGHIJ'

		this->setCursor(start, this->SecondLineY);
		this->write('>');
		this->setCursor(start + 1, this->SecondLineY);
		this->write(inPos);
		this->setCursor(start + 2, this->SecondLineY);
		this->write('<');

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
			this->setCursor(0, this->SecondLineY);
			this->print(LcdScreen::buffer);
			pos = strlen(LcdScreen::buffer);
		}

		this->GetString(LcdScreen::YesMsg);
		if (inChoiceValue != LcdScreen::NoMsg)
		{
			this->setCursor(pos, this->SecondLineY);
			this->write('>');
		}

		this->setCursor(pos + 1, this->SecondLineY);
		this->print(LcdScreen::buffer);

		if (inChoiceValue != LcdScreen::NoMsg)
		{
			this->setCursor(pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);
			this->write('<');
		}

		pos = pos + 1 + strlen(LcdScreen::buffer) + 1;

		this->GetString(LcdScreen::NoMsg);

		pos++;
		if (inChoiceValue == LcdScreen::NoMsg)
		{
			this->setCursor(pos, this->SecondLineY);
			this->write('>');
		}

		this->setCursor(pos + 1, this->SecondLineY);
		this->print(LcdScreen::buffer);

		if (inChoiceValue == LcdScreen::NoMsg)
		{
			this->setCursor(pos + 1 + strlen(LcdScreen::buffer), this->SecondLineY);
			this->write('<');
		}
	}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
