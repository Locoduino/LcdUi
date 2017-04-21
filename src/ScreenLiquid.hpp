//-------------------------------------------------------------------
#ifndef __screenLiquid_H__
#define __screenLiquid_H__
//-------------------------------------------------------------------

#ifndef _LCDVS_H_
#include "LiquidCrystal.h"
#endif

#ifndef LiquidCrystal_h
#error To be able to compile this screen, the 'LiquidCrystal' library must be installed.
#endif

#include "LcdScreen.hpp"

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
		for (unsigned int i = 0; i < this->sizex; i++)
		{
			this->setCursor(i, posy);
			this->write(' ');
		}
	}

	void begin(byte inSizeX, byte inSizeY, PGM_P const *inpStringTable, LiquidCrystal *inpLcd)
	{
		this->pLcd = inpLcd;
		this->pLcd->begin(inSizeX, inSizeY);

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
	inline void home() { SCREEN->home();	}
	inline void setCursor(byte Col, byte Row) { SCREEN->setCursor(Col, Row); }
	inline void display() { SCREEN->display(); }
	inline void noDisplay() { SCREEN->noDisplay(); }
	inline void cursor() { SCREEN->cursor(); }
	inline void noCursor() { SCREEN->noCursor(); }
	inline void blink() { SCREEN->blink(); }
	inline void noBlink() { SCREEN->noBlink(); }

	// LCD_API and LcdScreen override...
	inline void clear() { SCREEN->clear(); }

	// LcdScreen overrides...

	void DisplayChar(char car, byte inX, byte inY)
	{
		this->setCursor(inX, inY);
		this->write(car);
	}

	void DisplayText(char *inText, byte inX, byte inY)
	{
		this->setCursor(inX, inY);
		inText[this->sizex - inX] = 0;
		this->print(inText);
	}

	void DisplayCenteredText(char *inText, byte inY, bool inChoosen)
	{
		this->clearLine(inY);
		byte pos = (this->sizex / 2) - ((byte)strlen(inText) / 2) - 1;

		this->DisplayChar(inChoosen ? '>' : ' ', pos++, inY);
		this->DisplayText((char *)inText, pos, inY);
		pos += (byte)strlen(inText);
		this->DisplayChar(inChoosen ? '<' : ' ', pos, inY);
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
			int len = (int)strlen(LcdScreen::buffer);
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

		this->clearLine(line);
		this->DisplayCenteredText((char *)inChoice, line, inChoosen);
	}

	void DisplayInt(int inValue)
	{
		this->BuildString(inValue, LcdScreen::buffer);
		this->DisplayCenteredText(LcdScreen::buffer, this->SecondLineY, true);
	}

	void DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar)
	{
		this->noCursor();
		this->DisplayText((char *)inTextValue, 0, this->HeaderY);

		for (int i = (int)strlen(inTextValue); i < inLength && i < sizex; i++)
			this->DisplayChar('_', i, this->HeaderY);
	}

	void DisplayTextChoice(byte inPos, byte inEditedChar)
	{
		this->noCursor();
		this->setCursor(0, this->SecondLineY);

		//   'ABCD>E<FGHIJ'

		if (inPos == 0)
		{
			this->GetString(LcdScreen::BackspaceMsg);
			this->DisplayCenteredText(LcdScreen::buffer, this->SecondLineY, true);
		}
		else
		{
			char text[2];
			text[0] = inPos;
			text[1] = 0;
			this->DisplayCenteredText(text, this->SecondLineY, true);
		}
		this->setCursor(inEditedChar, this->HeaderY);
		this->cursor();
	}

	void DisplayYesNo(byte inChoiceValue, bool inOnlyYes, int inPrefixString = 0)
	{
		this->clearLine(this->SecondLineY);

		byte pos = 0;
		if (inPrefixString != 0)
		{
			this->GetString(inPrefixString);
			this->DisplayText(LcdScreen::buffer, 0, this->SecondLineY);
			pos = (byte)strlen(LcdScreen::buffer);
		}

		if (inOnlyYes || inChoiceValue != LcdScreen::NoMsg)
			this->DisplayChar('>', pos, this->SecondLineY);

		this->GetString(LcdScreen::YesMsg);
		this->DisplayText(LcdScreen::buffer, pos + 1, this->SecondLineY);

		if (inOnlyYes || inChoiceValue != LcdScreen::NoMsg)
			this->DisplayChar('<', pos + 1 + (byte)strlen(LcdScreen::buffer), this->SecondLineY);

		if (!inOnlyYes)
		{
			pos = pos + 1 + (byte)strlen(LcdScreen::buffer) + 1;

			this->GetString(LcdScreen::NoMsg);

			pos++;
			if (inChoiceValue == LcdScreen::NoMsg)
				this->DisplayChar('>', pos, this->SecondLineY);

			this->DisplayText(LcdScreen::buffer, pos + 1, this->SecondLineY);

			if (inChoiceValue == LcdScreen::NoMsg)
				this->DisplayChar('<', pos + 1 + (byte)strlen(LcdScreen::buffer), this->SecondLineY);
		}
	}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
