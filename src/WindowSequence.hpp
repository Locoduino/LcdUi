//-------------------------------------------------------------------
#ifndef __windowSequence_H__
#define __windowSequence_H__
//-------------------------------------------------------------------

#include "Window.hpp"
#include "Chain.hpp"

#define WINDOW_SEQUENCE_NB_FATHERS	5

//-------------------------------------------------------------------

// Describes a sequency of windows

class WindowSequence : public Window
{
private:
	LCDUICHAINEDLIST<Window> windows;
	byte Fathers[WINDOW_SEQUENCE_NB_FATHERS];
	byte FathersOption[WINDOW_SEQUENCE_NB_FATHERS];

public:
	WindowSequence();

	inline byte GetType() const { return WINDOWTYPE_SEQUENCE; }
	inline void begin(byte inId) { Window::begin(inId); }
	inline void AddWindow(Window *inWindow) { this->windows.AddItem(inWindow); }
	void AddFather(byte inFather, byte inOption);
	Window *GetNextWindow(Window *inpCurrentWindow);
	Window *GetPrevWindow(Window *inpCurrentWindow);
	Window *GetNextWindowWithFather(Window *inpCurrentWindow, byte inFatherToFindId, byte inChoiceToFind);
	Window *GetPrevWindowWithFather(Window *inpCurrentWindow, byte inFatherToFindId, byte inChoiceToFind);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
