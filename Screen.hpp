//-------------------------------------------------------------------
#ifndef __screen_H__
#define __screen_H__
//-------------------------------------------------------------------

class ScreenVS;

#define BUFFER_SIZE	40

//-------------------------------------------------------------------

class Screen
{
protected:
	const char * const *pStringTable;
	byte sizex, sizey;

public:
	static char buffer[BUFFER_SIZE];
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
#ifndef VISUALSTUDIO
	inline virtual void print(const __FlashStringHelper *) {}
#endif
	inline virtual void print(const char *) {}
	inline virtual void print(int , char ) {}	// DEC, HEX, BIN ...
	inline virtual void write(char ) {}
	inline virtual void clear() {}
	inline virtual void home() {}
	inline virtual void setCursor(byte , byte ) {}
	inline virtual void cursor_on() {}
	inline virtual void cursor_off() {}
	inline virtual void blink_on() {}
	inline virtual void blink_off() {}
	/////////////

	// String building in global buffer
	static void BuildString(int inValue, char *outString, int digits = 0);
	static void BuildProgress(byte inValue, byte inMax, bool inFromLeftDir, byte inLengthString, char *outString);
	static byte BuildStringLeft(const char *inString, byte inSizeMax, char *outString);
	static byte GetChar(int inPos);

	// Get real string from pStringTable.
	char *GetString(int inString);
	char *GetChoiceString(int inString);

	// Display string in contexts
	inline virtual void DisplayHeader(int) {}
	inline virtual void DisplayChoice(int) {}
	inline virtual void DisplayChoice(const char *) {}
	inline virtual void DisplayInt(int ) {}
	inline virtual void DisplayTextResult(const char *, byte , byte ) {}
	inline virtual void DisplayTextChoice(byte , byte ) {}
	inline virtual void DisplayYesNo(byte , int = 0) {}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
