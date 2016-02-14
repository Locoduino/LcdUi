
// LCDUI_VStudioMfcDlg.h : header file
//

#pragma once

#include "stdafx.h"
#include "resource.h"		// main symbols

#define DEBUGLINES	10

// CLCDUI_VStudioMfcDlg dialog
class CLCDUI_VStudioMfcDlg : public CDialogEx
{
// Construction
public:
	CLCDUI_VStudioMfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LCDUI_VSTUDIOMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CFont font;
	char debugs[DEBUGLINES][200];
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void Setup(byte inSizex, byte inSizey);
	void clear();
	void print(byte inLine, const char *inString);
	void getLine(byte inLine, char *inBuffer, int inLength);
	void debug(const char *inString);
	afx_msg void OnBnClickedStart();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	static CLCDUI_VStudioMfcDlg *pMainDialog;
};
