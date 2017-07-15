/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a sequence of windows>
*************************************************************/

#include "LcdUi.h"

WindowSequence::WindowSequence()
{
	for (int i = 0; i < WINDOW_SEQUENCE_NB_FATHERS; i++)
		this->Fathers[i] = this->FathersOption[i] = 255;	
}

Window *WindowSequence::GetNextWindow(Window *inpCurrentWindow)
{
	LCDUICHAINEDLISTITEM<Window> *pCurr = this->windows.pFirst;

	if (inpCurrentWindow == NULL)
		return pCurr->Obj;

	// Find index of current item.
	while (pCurr != NULL)
	{
		if (pCurr->Obj == inpCurrentWindow)
		{
			if (pCurr->pNext == NULL)
				return NULL;
			return pCurr->pNext->Obj;
		}
		pCurr = pCurr->pNext;
	}

	return NULL;
}

Window *WindowSequence::GetPrevWindow(Window *inpCurrentWindow)
{
	Window *pPrev = NULL;
	LCDUICHAINEDLISTITEM<Window> *pCurr = this->windows.pFirst;

	// Find index of current item.
	while (pCurr != NULL)
	{
		if (inpCurrentWindow != NULL)
		{
			if (pCurr->Obj == inpCurrentWindow)
			{
				if (pCurr->pNext == NULL)
					return NULL;
				return pPrev;
			}
		}

		pPrev = pCurr->Obj;
		pCurr = pCurr->pNext;
	}

	if (inpCurrentWindow == NULL)
		return pPrev;

	return NULL;
}

Window *WindowSequence::GetNextWindowWithFather(Window *inpCurrentWindow, byte inFatherToFindId, byte inChoiceToFind)
{
	return NULL;
}

Window *WindowSequence::GetPrevWindowWithFather(Window *inpCurrentWindow, byte inFatherToFindId, byte inChoiceToFind)
{
	return NULL;
}

void WindowSequence::AddFather(byte inFather, byte inOption)
{
	for (int i = 0; i < WINDOW_SEQUENCE_NB_FATHERS; i++) 
	{
		if (this->Fathers[i] != 255)
		{
			this->Fathers[i] = inFather;
			this->FathersOption[i] = inOption;
			return;
		}
	}
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowSequence::printWindow()
{
	printWindowHeader(F("Window Sequence"));
	Serial.println("");
}
#endif