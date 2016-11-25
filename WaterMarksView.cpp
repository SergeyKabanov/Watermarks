
// WaterMarksView.cpp : implementation of the CWaterMarksView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaterMarks.h"
#endif

#include "WaterMarksDoc.h"
#include "WaterMarksView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CWaterMarksView* g_pWaterMarksView;
extern CMainFrame* g_pMainFrame;
extern CWaterMarksApp* p_OurApp;

// CWaterMarksView

IMPLEMENT_DYNCREATE(CWaterMarksView, CView)

BEGIN_MESSAGE_MAP(CWaterMarksView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CWaterMarksView construction/destruction
//###############################################################################################################
CWaterMarksView::CWaterMarksView()
{
	// TODO: add construction code here
	g_pWaterMarksView = this;
	m_iAlpha = 100;
	m_cTransparentColor = RGB(255, 255, 255);
	m_iDrawCounter = 0;
	m_bFirstTime = false;
	m_WatermarkPosition = E_WATERMARK_CENTER;
}
//###############################################################################################################
CWaterMarksView::~CWaterMarksView()
{
}
//###############################################################################################################
BOOL CWaterMarksView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
//###############################################################################################################
// CWaterMarksView drawing
//##############################################################################################################
void CWaterMarksView::OnDraw(CDC* pDC)
{
	CWaterMarksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect OurRect;
	GetClientRect(&OurRect);

	CSize OurSize = GetTotalSize();
	if (OurRect.right<OurSize.cx) OurRect.right = OurSize.cx;
	if (OurRect.bottom<OurSize.cy) OurRect.bottom = OurSize.cy;

	CBrush OurBrush(g_pMainFrame->m_clrBkgnd);
	pDC->FillRect(&OurRect, &OurBrush);
//......................................................................................................................

// Render to memory DC to prevent flickering
	CRect rcClient;
	CRect rcImg1, rcImg2;

	GetClientRect(&rcClient);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBmp);

	// Fill background with background color
	memDC.FillSolidRect(&rcClient, g_pMainFrame->m_clrBkgnd);

	if (m_img1.IsLoaded())
	{

		UINT nImgWidth = m_img1.GetWidth();
		UINT nImgHeight = m_img1.GetHeight();

		float fImgAspect = nImgWidth / float(nImgHeight);
		float fDlgAspect = rcClient.Width() / float(rcClient.Height());

		rcImg1 = rcClient;

			if (fImgAspect > fDlgAspect)
			{
				int iNewHeight = int(rcClient.Width() / fImgAspect + 0.5);
				rcImg1.top = int((rcClient.Height() - iNewHeight) / 2.0 + 0.5);
				rcImg1.bottom = rcImg1.top + iNewHeight;
			}
			else if (fImgAspect < fDlgAspect)
			{
				int iNewWidth = int(rcClient.Height() * fImgAspect + 0.5);
				rcImg1.left = int((rcClient.Width() - iNewWidth) / 2.0 + 0.5);
				rcImg1.right = rcImg1.left + iNewWidth;
			}
		m_img1.Render(memDC.GetSafeHdc(), rcImg1.left, rcImg1.top, rcImg1.Width(), rcImg1.Height());
	}

	if (m_img2.IsLoaded())
	{
		if (m_WatermarkPosition == E_WATERMARK_CENTER)
		{
			if (rcClient.right < (LONG)m_img2.GetWidth())
				rcImg2.left = 0;
			else
			rcImg2.left = int((rcClient.right - m_img2.GetWidth())/2.0-0.5);  
			
			if (rcClient.bottom < (LONG)m_img2.GetHeight())
				rcImg2.top = 0;
			else
			rcImg2.top = int((rcClient.bottom - m_img2.GetHeight()) / 2.0 - 0.5);

			rcImg2.right = rcImg2.left + m_img2.GetWidth();
			rcImg2.bottom = rcImg2.top + m_img2.GetHeight();
		}
		else
			if (m_WatermarkPosition == E_WATERMARK_PAGE)
			{
				rcImg2 = rcImg1;
			}
		else
		if (m_WatermarkPosition == E_WATERMARK_FIT)
		{

			UINT nImgWidth = m_img2.GetWidth();
			UINT nImgHeight = m_img2.GetHeight();

			float fImgAspect = nImgWidth / float(nImgHeight);
			float fDlgAspect = rcImg1.Width() / float(rcImg1.Height());

			rcImg2 = rcImg1;

			if (fImgAspect > fDlgAspect)
			{
				int iNewHeight = int(rcImg1.Width() / fImgAspect + 0.5);
				rcImg2.top = int((rcImg1.Height() - iNewHeight) / 2.0 + 0.5);
				if (rcImg2.top < rcImg1.top) rcImg2.top = rcImg1.top;
				rcImg2.bottom = rcImg2.top + iNewHeight;
			}
			else if (fImgAspect < fDlgAspect)
			{
				int iNewWidth = int(rcImg1.Height() * fImgAspect + 0.5);
				rcImg2.left = int((rcImg1.Width() - iNewWidth) / 2.0 + 0.5);
				if (rcImg2.left < rcImg1.left) rcImg2.left = rcImg1.left;
				rcImg2.right = rcImg2.left + iNewWidth;
			}
		}

		m_img2.RenderAlpha(memDC.GetSafeHdc(), rcImg2.left, rcImg2.top, rcImg2.Width(), rcImg2.Height(), m_iAlpha, m_cTransparentColor);
	}

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);

//...............................................................................................................
	if (!m_bFirstTime)
	{
		if(!p_OurApp->m_bDoNotLoadFiles)
		    SetTimer(0, 100, NULL);
		m_bFirstTime = true;
	}
}
//##############################################################################################################
// CWaterMarksView printing
//##############################################################################################################
BOOL CWaterMarksView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}
//##############################################################################################################
void CWaterMarksView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
//##############################################################################################################
void CWaterMarksView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
//##############################################################################################################
// CWaterMarksView diagnostics
//##############################################################################################################
#ifdef _DEBUG
void CWaterMarksView::AssertValid() const
{
	CView::AssertValid();
}
//##############################################################################################################
void CWaterMarksView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
//##############################################################################################################
CWaterMarksDoc* CWaterMarksView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaterMarksDoc)));
	return (CWaterMarksDoc*)m_pDocument;
}
#endif //_DEBUG
//##############################################################################################################
// CWaterMarksView message handlers
//##############################################################################################################
void CWaterMarksView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	//Sleep(5000);
}
//##############################################################################################################
bool CWaterMarksView::LoadBackgroundImage()
{
	CString sOurString;

	// Load the files from CommandString:
	if (!DoesFileExist(p_OurApp->sFirstFileName))
	{
		KillTimer(0);
		sOurString.Format(_T("Cannot find the background file: %s"), p_OurApp->sFirstFileName);
		AfxMessageBox(sOurString);
		return false;
	}

	try
	{
		m_img1.Open(p_OurApp->sFirstFileName, true);
		Invalidate();
	}
	catch (HRESULT hr)
	{
		CString str;
		KillTimer(0);
		str.Format(_T("Error 0x%X while loading background image: '%s'"), hr, p_OurApp->sFirstFileName);
		AfxMessageBox(str, MB_ICONERROR);
		return false;
	}

	return true;
}
//##############################################################################################################
bool CWaterMarksView::LoadWatermarkImage()
{
	CString sOurString;

	// Load the files from CommandString:

	if (!DoesFileExist(p_OurApp->sSecondFileName))
	{
		KillTimer(0);
		sOurString.Format(_T("Cannot find the watermark file: %s"), p_OurApp->sSecondFileName);
		AfxMessageBox(sOurString);
		return false;
	}

	
	try
	{
		m_img2.Open(p_OurApp->sSecondFileName, true);
		Invalidate();
	}
	catch (HRESULT hr)
	{
		CString str;
		KillTimer(0);
		str.Format(_T("Error 0x%X while loading watermark image: '%s'"), hr, p_OurApp->sSecondFileName);
		AfxMessageBox(str, MB_ICONERROR);
		return false;
	}

	return true;
}
//##############################################################################################################
bool CWaterMarksView::DoesFileExist(const CString & OurFileName)
{
	CFileFind finder;
	BOOL bWorking;
	bWorking = finder.FindFile(OurFileName);
	if (bWorking)
		return true;
	else
		return false;
}
//###################################################################################################################
int CWaterMarksView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CWaterMarksDoc *pDoc = GetDocument();

	if (!pDoc)
	{
		return -1;
	}

	pDoc->SetTitle(_T("WATERMARKS"));

	m_pOptionsForm = COptionsForm::CreateDynamic(this);
	//m_pOptionsForm->Init();

	return 0;
}
//##############################################################################################################
void CWaterMarksView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_iDrawCounter == 0)
	{
		if(!LoadBackgroundImage())
		    KillTimer(nIDEvent);
		
		++m_iDrawCounter;
		return;
	}

	if (m_iDrawCounter == 1)
	{
		LoadWatermarkImage();
		KillTimer(nIDEvent); // do it anyway - no more events coming
		++m_iDrawCounter;
		return;
	}

	CScrollView::OnTimer(nIDEvent);
}
//##############################################################################################################