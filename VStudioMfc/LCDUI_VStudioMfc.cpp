// LCDUI_VStudioMfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "conio.h"
#include "LCDUI_VStudioMfc.h"
#include "LCDUI_VStudioMfcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void setup();
void loop();

// CLCDUI_VStudioMfcApp

BEGIN_MESSAGE_MAP(CLCDUI_VStudioMfcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CLCDUI_VStudioMfcApp construction

CLCDUI_VStudioMfcApp::CLCDUI_VStudioMfcApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CLCDUI_VStudioMfcApp object

CLCDUI_VStudioMfcApp theApp;


// CLCDUI_VStudioMfcApp initialization

BOOL CLCDUI_VStudioMfcApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CLCDUI_VStudioMfcDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	lastKeyPressed = 0;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CLCDUI_VStudioMfcApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	// Check to make sure CPenWidthsDlg is up
	if (m_pMainWnd->m_hWnd != NULL)
	{
		if ((lpMsg->hwnd == m_pMainWnd->m_hWnd) ||
			::IsChild(m_pMainWnd->m_hWnd, lpMsg->hwnd))
			// Use ::IsChild to get messages that may be going
			// to the dialog's controls.  In the case of
			// WM_KEYDOWN this is required.
		{
			if (lpMsg->message == WM_CHAR)
				lastKeyPressed = lpMsg->wParam;
		}
	}
	// Default processing of the message.
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

//-----------------------------------------
//			ino PART
//-----------------------------------------
#ifdef DCDCCCONTROLER
#include "..\DcDccControler\VStudioLcdUi\LcdUi_ino.hpp"
#else
#include "LcdUi.ino"
#endif
//-----------------------------------------
//			end ino PART
//-----------------------------------------
