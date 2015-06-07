//-------------------------------------------------------------------
#ifndef __screen_H__
#define __screen_H__
//-------------------------------------------------------------------

class ScreenVS;

//-------------------------------------------------------------------

class Screen
{
protected:
	const char * const *pStringTable;
	byte sizex, sizey;

public:
	static char buffer[40];
	static int YesMsg;
	static int NoMsg;

#ifdef VISUALSTUDIO
	ScreenVS *pScreenVS;
#endif

	inline Screen() { this->pStringTable = 0; this->sizex = 0; this->sizey = 0; }

	virtual void Setup(byte inSizeX, byte inSizeY, const char * const *inpStringTable);
	inline byte GetSizeX() const { return this->sizex; }
	inline byte GetSizeY() const { return this->sizey; }
	void clearLine(int posy);

	// LCD API
	//LibraryName
	inline virtual void init() { }
	inline virtual void setDelay(int inCmdDelay, int inCharDelay) {}
#ifndef VISUALSTUDIO
	inline virtual void print(const __FlashStringHelper *inString) {}
#endif
	inline virtual void print(const char *inString) {}
	inline virtual void print(int inValue, char inType) {}	// DEC, HEX, BIN ...
	inline virtual void write(char inValue) {}
	inline virtual void clear() {}
	inline virtual void home() {}
	inline virtual void setCursor(byte Col, byte Row) {}
	inline virtual void cursor_on() {}
	inline virtual void cursor_off() {}
	inline virtual void blink_on() {}
	inline virtual void blink_off() {}
	/////////////

	static void BuildString(int inValue, char *outString, int digits = 0);
	static void BuildProgress(byte inValue, byte inMax, bool inFromLeftDir, byte inLengthString, char *outString);
	static byte BuildStringLeft(const char *inString, byte inSizeMax, char *outString);
	static byte GetChar(int inPos);

	char *GetString(int inString);

	inline virtual void DisplayHeader(int inHeader) {}
	inline virtual void DisplayChoice(int inCurrentChoice) {}
	inline virtual void DisplayChoice(const char *inChoice) {}
	inline virtual void DisplayInt(int inValue) {}
	inline virtual void DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar) {}
	inline virtual void DisplayTextChoice(byte inPos, byte inEditedChar) {}
	inline virtual void DisplayYesNo(byte inChoiceValue, int prefixString = 0) {}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
