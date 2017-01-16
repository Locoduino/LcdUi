//-------------------------------------------------------------------
#ifndef __screen_H__
#define __screen_H__
//-------------------------------------------------------------------

#define BUFFER_SIZE	40

//-------------------------------------------------------------------

class LcdScreen
{
protected:
	const char * const *pStringTable;
	byte sizex, sizey;

public:
	static char buffer[BUFFER_SIZE];
	static int YesMsg;
	static int NoMsg;
	static int BackspaceMsg;
	static int ValidateMsg;
	byte HeaderY;
	byte SecondLineY;

	byte FirstChoiceShown;	 

	virtual void begin(byte inSizeX, byte inSizeY, const char * const *inpStringTable);
	inline byte GetSizeX() const { return this->sizex; }
	inline byte GetSizeY() const { return this->sizey; }

	// String building in global buffer
	static void BuildString(int inValue, char *outString, int digits = 0);
	static void BuildProgress(byte inValue, byte inMax, bool inFromLeftDir, byte inLengthString, char *outString);
	static byte BuildStringLeft(const char *inString, byte inSizeMax, char *outString);
	static byte GetChar(int inPos);

	// Get real string from pStringTable.
	char *GetString(int inString);
	char *GetChoiceString(int inString);

	// Display string in contexts
	inline virtual void clear() {};
	inline virtual void DisplayHeader(int inHeader) {}
	inline void DisplayText(int inText, byte inX, byte inY) { this->DisplayText(this->GetString(inText), inX, inY); }
	inline virtual void DisplayText(char *inText, byte inX, byte inY) {}
	inline virtual void DisplayChoice(int inChoice, int inIndex, bool inShowIndex, bool inSelected) {}
	inline virtual void DisplayChoice(const char *inChoice, int inIndex, bool inChoosen) {}
	inline virtual void DisplayInt(int inValue) {}
	inline virtual void DisplayTextResult(const char *inTextValue, byte inLength, byte inEditedChar) {}
	inline virtual void DisplayTextChoice(byte inPos, byte inEditedChar) {}
	inline virtual void DisplayYesNo(byte inChoiceValue, int prefixString = 0) {}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
