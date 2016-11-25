
// WaterMarks.h : main header file for the WaterMarks application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CWaterMarksApp:
// See WaterMarks.cpp for the implementation of this class
//

class CWaterMarksApp : public CWinApp
{
public:
	CWaterMarksApp();

	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;

	CString sFirstFileName;
	CString sSecondFileName;
	bool m_bDoNotLoadFiles;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void GetFileNames(const CString & sInput);

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWaterMarksApp theApp;
