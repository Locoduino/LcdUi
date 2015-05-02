//-------------------------------------------------------------------
#ifndef __screenTwoLinesVS_H__
#define __screenTwoLinesVS_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

//#include "Screen.hpp"

//-------------------------------------------------------------------

class ScreenVS
{
private:
	byte sizex, sizey;
	byte posx, posy;
	bool cursorOn, blinkOn;

public:
	ScreenVS() { cursorOn = false; blinkOn = false; }

	void Setup(byte inSizeX, byte inSizeY);

	void printVS(int posy, const char *inString);
	void clearVS();
	void delayVS(int value);

	// LCD API
	//LibraryName
	inline void init() { clearVS(); }
	inline void setDelay(int inCmdDelay, int inCharDelay) {}
	void print(const char *inString);
	void print(int inValue, char inType);	// DEC, HEX, BIN ...
	void write(char inValue);
	inline void command(const char *inCmd) {}
	void clear();
	inline void home() { setCursor(0, 0); }
	inline void setCursor(byte Col, byte Row) { this->posy = Row; this->posx = Col; }
	void cursor_on();
	void cursor_off();
	void blink_on();
	void blink_off();
	/////////////

};

//-------------------------------------------------------------------

//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their  
// 		declarations into this file.
// 		If you do not want to use this automated process,  
//		simply delete the lines below, with "&MM_DECLA" text 
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
