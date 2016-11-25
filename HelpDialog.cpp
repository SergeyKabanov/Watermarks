// HelpDialog.cpp : implementation file

#include "stdafx.h"
#include "WaterMarks.h"
#include "HelpDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#pragma warning(disable:4311)
#pragma warning(disable:4302)

extern CMainFrame* g_pMainFrame;
DWORD CALLBACK MyStreamInCallback(CFile* dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
// CHelpDialog dialog

IMPLEMENT_DYNAMIC(CHelpDialog, CDialog)

CHelpDialog::CHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HELP_DIALOG, pParent)
{

}
//#####################################################################################################################
CHelpDialog::~CHelpDialog()
{
}
//#####################################################################################################################
void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//#####################################################################################################################
BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//#####################################################################################################################
// CHelpDialog message handlers
//#####################################################################################################################
BOOL CHelpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	VERIFY(m_RichEdit.SubclassDlgItem(IDC_RICH_EDIT_HELP, this));
	
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE | CFM_EFFECTS;
	cf.dwEffects = CFM_BOLD | CFM_ITALIC;
	cf.crTextColor = LIGHTBLUE;
	cf.yHeight = 250;
	m_RichEdit.SetDefaultCharFormat(cf);
	
	CString FileName = _T("readme.txt");

	// Convert full filename characters from "\" to "\\"
	//FileName.Replace(_T("\\"), _T("\\\\"));

	// The file from which to load the contents of the rich edit control.
	CFile cFile(FileName, CFile::modeRead);
	EDITSTREAM es;

	es.dwCookie = (DWORD)&cFile;
	es.pfnCallback = (EDITSTREAMCALLBACK)MyStreamInCallback;

	m_RichEdit.StreamIn(SF_TEXT, es); // Perform the streaming

	return TRUE;  // return TRUE unless you set the focus to a control
}
//##############################################################################################################
DWORD CALLBACK MyStreamInCallback(CFile* dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Required for StreamIn
	CFile* pFile = (CFile*)dwCookie;

	*pcb = pFile->Read(pbBuff, cb);

	return 0;
}
//#####################################################################################################################
void CHelpDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect OurRect;
	GetClientRect(&OurRect);

	CBrush OurBrush(g_pMainFrame->m_clrBkgnd);
	dc.FillRect(&OurRect, &OurBrush);
}
//#####################################################################################################################
