// ColorEdit.cpp : implementation file
//
#include "stdafx.h"
#include "ColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//######################################################################################################################33####################
CColorEdit::CColorEdit()
{
	m_clrText=BLUE;
	m_clrBkgnd=LIGHT_LIGHT_GREEN;
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd); 
}
//#############################################################################################################################################
CColorEdit::~CColorEdit()
{
}
//#############################################################################################################################################
BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
        ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
//######################################################################################################################33####################
// CColorEdit message handlers
//######################################################################################################################33####################
HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor(m_clrText);
    pDC->SetBkColor(m_clrBkgnd);
    return m_brBkgnd;
}
//#################################################################################################################################
void CColorEdit::SetColors(COLORREF TextColor, COLORREF BackgroundColor)
{
	m_clrText=TextColor;
	m_clrBkgnd=BackgroundColor;
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd); 
}
//#############################################################################################################################################
