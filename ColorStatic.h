#if !defined(AFX_COLORSTATIC_H__0998555C_F826_4558_BD1A_4003BB0C1C68__INCLUDED_)
#define AFX_COLORSTATIC_H__0998555C_F826_4558_BD1A_4003BB0C1C68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Attributes
public:
    COLORREF m_clrText;
    COLORREF m_clrBkgnd;
    CBrush   m_brBkgnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__0998555C_F826_4558_BD1A_4003BB0C1C68__INCLUDED_)
