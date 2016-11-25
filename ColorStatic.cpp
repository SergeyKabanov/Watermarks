// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic
//###############################################################################################################
CColorStatic::CColorStatic()
{
	m_clrText=LIGHTBLUE;
	m_clrBkgnd=LIGHT_LIGHT_GREEN;
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd); 
}
//#############################################################################################################
CColorStatic::~CColorStatic()
{
}
//#############################################################################################################
BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
//{{AFX_MSG_MAP(CColorStatic)
// NOTE - the ClassWizard will add and remove mapping macros here.
ON_WM_CTLCOLOR_REFLECT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers
//######################################################################################################
HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
#pragma warning (disable : 4100)
	//if(CWnd::GetFocus()==this)
	//	{
    pDC->SetTextColor(m_clrText);
    pDC->SetBkColor(m_clrBkgnd);
    return m_brBkgnd;
	//	}
	//return NULL;
}
//#######################################################################################################
