//-------------------------------------------------------------------
#ifndef __windowInt_H__
#define __windowInt_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

// Change an integer value

/// +------------------+
/// | Address          |
/// |       >12<       |
/// |                  |
/// |                  |
/// +------------------+

class WindowInt : public Window
{
protected:
	int minIntValue;
	int maxIntValue;

	// values for edition modes
	int *pValue;

public:
	inline WindowInt() { this->minIntValue = this->maxIntValue = 0; this->pValue = NULL; }
	inline WindowInt(byte inFirstLine, int *inpValue, int inMinIntValue = 0, int inMaxIntValue = 32767) { this->begin(inFirstLine, inpValue, inMinIntValue, inMaxIntValue); }

	void begin(byte inFirstLine, int *inpValue, int inMinIntValue = 0, int inMaxIntValue = 32767);
	inline byte GetType() const { return WINDOWTYPE_INT; }
	inline int GetMinIntValue() const { return this->minIntValue; }
	inline int GetMaxIntValue() const { return this->maxIntValue; }
	inline void SetMinIntValue(int inMin) { this->minIntValue = inMin; }
	inline void SetMaxIntValue(int inMax) { this->maxIntValue = inMax; }

	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
