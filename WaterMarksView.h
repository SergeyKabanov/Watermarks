
// WaterMarksView.h : interface of the CWaterMarksView class
//

#pragma once
#include "WaterMarksDoc.h"
#include "WicImage.h"
#include "OptionsForm.h"

class CWaterMarksView : public CScrollView
{
protected: // create from serialization only
	CWaterMarksView();
	DECLARE_DYNCREATE(CWaterMarksView)

// Attributes
public:
	CWaterMarksDoc* GetDocument() const;
	COptionsForm* m_pOptionsForm;

	CWicImage m_img1;
	CWicImage m_img2;

	int m_iAlpha;
	COLORREF m_cTransparentColor;

	T_WATERMARK_POSITION m_WatermarkPosition;

// Operations
public:
	bool LoadBackgroundImage();
	bool LoadWatermarkImage();
	bool DoesFileExist(const CString & OurFileName);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWaterMarksView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_iDrawCounter;
	bool m_bFirstTime;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in WaterMarksView.cpp
inline CWaterMarksDoc* CWaterMarksView::GetDocument() const
   { return reinterpret_cast<CWaterMarksDoc*>(m_pDocument); }
#endif

