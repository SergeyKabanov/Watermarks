#if !defined(AFX_COLOREDIT_H__134D1453_73AA_4E62_9942_534B1D936AC9__INCLUDED_)
#define AFX_COLOREDIT_H__134D1453_73AA_4E62_9942_534B1D936AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorEdit window

class CColorEdit : public CEdit
{
// Construction
public:
	CColorEdit();

// Attributes
public:
    COLORREF m_clrText;
    COLORREF m_clrBkgnd;
    CBrush   m_brBkgnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorEdit();
    void SetColors(COLORREF TextColor, COLORREF BackgroundColor);

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
     	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDIT_H__134D1453_73AA_4E62_9942_534B1D936AC9__INCLUDED_)
