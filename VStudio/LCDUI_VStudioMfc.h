
// LCDUI_VStudioMfc.h : main header file for the PROJECT_NAME application
//

#pragma once


#include "stdafx.h"		// main symbols
#include "resource.h"		// main symbols
#include "LCDUI_VStudioMfcDlg.h"

// CLCDUI_VStudioMfcApp:
// See LCDUI_VStudioMfc.cpp for the implementation of this class
//

class CLCDUI_VStudioMfcApp : public CWinApp
{
public:
	char lastKeyPressed;

	CLCDUI_VStudioMfcApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CLCDUI_VStudioMfcDlg *GetDialog() { return (CLCDUI_VStudioMfcDlg *)this->m_pMainWnd; }
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLCDUI_VStudioMfcApp theApp;

void setup();
void loop();