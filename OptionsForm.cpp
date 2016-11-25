// OptionsForm.cpp : implementation file
//

#include "stdafx.h"
#include "WaterMarks.h"
#include "OptionsForm.h"
#include "WaterMarksView.h"
#include "MainFrm.h"
#include "HelpDialog.h"

extern CWaterMarksApp* p_OurApp;
extern CWaterMarksView* g_pWaterMarksView;
extern CMainFrame* g_pMainFrame;

// COptionsForm

IMPLEMENT_DYNCREATE(COptionsForm, CFormView)

COptionsForm::COptionsForm()
	: CFormView(IDD_PROPERTIES)
{
	bFirstTime = false;
}

COptionsForm::~COptionsForm()
{

}

void COptionsForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COptionsForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_BACKGROUND_IMAGE, &COptionsForm::OnBnClickedButtonLoadBackgroundImage)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_WATERMARK_IMAGE2, &COptionsForm::OnBnClickedButtonLoadWatermarkImage2)
	ON_BN_CLICKED(IDC_BUTTON_SET_TRANSPARENCY_LEVEL, &COptionsForm::OnBnClickedButtonSetTransparencyLevel)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_TRANSPARENT_COLOR, &COptionsForm::OnBnClickedButtonChooseTransparentColor)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &COptionsForm::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_RADIO_CENTER, &COptionsForm::OnBnClickedRadioCenter)
	ON_BN_CLICKED(IDC_RADIO_FIT_TO_PAGE, &COptionsForm::OnBnClickedRadioFitToPage)
	ON_BN_CLICKED(IDC_RADIO_SCALE_TO_FIT, &COptionsForm::OnBnClickedRadioScaleToFit)
END_MESSAGE_MAP()


// COptionsForm diagnostics

#ifdef _DEBUG
void COptionsForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionsForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
//############################################################################################################
COptionsForm *COptionsForm::CreateDynamic(CWnd *pParent)
{
	COptionsForm *pFormView = new COptionsForm();

	//CDockableFormViewAppDoc *pDoc = CDockableFormViewAppDoc::CreateOne();
	//pFormView->m_pDocument = pDoc;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	CCreateContext *pContext = NULL;

#if 0
	if (!pFormView->CreateEx(0, NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 205, 157),
		pParent, -1, pContext))
#else
	if (!pFormView->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectDummy, pParent, 0, pContext))
#endif
		//if( !pFormView->CreateEx( 0, AfxRegisterWndClass(0, 0, 0, 0), NULL,
		//  WS_CHILD | WS_VISIBLE, CRect( 0, 0, 205, 157), pParent, -1, pContext) )
	{
		AfxMessageBox(_T("Failed in creating CMyFormView"));
		return NULL;
	}

	//pFormView->OnInitialUpdate();

	return pFormView;
}
//############################################################################################################
// COptionsForm message handlers
//############################################################################################################
void COptionsForm::Init()
{
	CString sOurString;

	sOurString.Format(_T("%d"),g_pWaterMarksView->m_iAlpha);
	SetDlgItemText(IDC_EDIT_TRANSPARENCY_LEVEL, sOurString);

	VERIFY(m_RadioCenter.SubclassDlgItem(IDC_RADIO_CENTER, this));
	VERIFY(m_RadioPage.SubclassDlgItem(IDC_RADIO_FIT_TO_PAGE, this));
	VERIFY(m_RadioScale.SubclassDlgItem(IDC_RADIO_SCALE_TO_FIT, this));

	switch (g_pWaterMarksView->m_WatermarkPosition)
	{
	case E_WATERMARK_CENTER:
		m_RadioCenter.SetCheck(1);
		m_RadioPage.SetCheck(0);
		m_RadioScale.SetCheck(0);
		break;

	case E_WATERMARK_PAGE:
		m_RadioCenter.SetCheck(0);
		m_RadioPage.SetCheck(1);
		m_RadioScale.SetCheck(0);
		break;

	case E_WATERMARK_FIT:
		m_RadioCenter.SetCheck(0);
		m_RadioPage.SetCheck(0);
		m_RadioScale.SetCheck(1);
		break;
	}
}
//##############################################################################################################
void COptionsForm::OnDraw(CDC* pDC)
{
	// TODO: Add your specialized code here and/or call the base class
	CRect OurRect;
	GetClientRect(&OurRect);

	CSize OurSize = GetTotalSize();
	if (OurRect.right<OurSize.cx) OurRect.right = OurSize.cx;
	if (OurRect.bottom<OurSize.cy) OurRect.bottom = OurSize.cy;

	CBrush OurBrush(g_pMainFrame->m_clrBkgnd);
	pDC->FillRect(&OurRect, &OurBrush);
//.................................................................................................................
	if (!bFirstTime)
	{
		bFirstTime = true;
		Init();
	}
	
}
//##############################################################################################################
void COptionsForm::OnBnClickedButtonLoadBackgroundImage()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		try
		{
			g_pWaterMarksView->m_img1.Open(dlg.GetPathName(),true);
			g_pWaterMarksView->RedrawWindow();
		}
		catch (HRESULT hr)
		{
			CString str;
			str.Format(_T("Error 0x%X while loading image '%s'"), hr, dlg.GetPathName());
			AfxMessageBox(str, MB_ICONERROR);
		}
	}
}
//##############################################################################################################
void COptionsForm::OnBnClickedButtonLoadWatermarkImage2()
{
	// TODO: Add your control notification handler code here

	if ((g_pWaterMarksView->m_img1.GetWidth() == 0) && (g_pWaterMarksView->m_img1.GetHeight() == 0))
	{
		AfxMessageBox(_T("Please load the background image first!"));
		return;
	}

	CFileDialog dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		try
		{
			g_pWaterMarksView->m_img2.Open(dlg.GetPathName(), true);
			g_pWaterMarksView->RedrawWindow();
		}
		catch (HRESULT hr)
		{
			CString str;
			str.Format(_T("Error 0x%X while loading image '%s'"), hr, dlg.GetPathName());
			AfxMessageBox(str, MB_ICONERROR);
		}
	}
}
//##############################################################################################################
void COptionsForm::OnBnClickedButtonSetTransparencyLevel()
{
	CString sOurString;
	bool bCorrect;
	int i, iAlpha;
	TCHAR c;

	GetDlgItemText(IDC_EDIT_TRANSPARENCY_LEVEL, sOurString);
	bCorrect = true;
	for (i = 0; i < sOurString.GetLength(); ++i)
	{
		c = sOurString.GetAt(i);
		if (!IsItNumber(c))
		{
			bCorrect = false;
			break;
		}
	}

	if (!bCorrect)
	{
		AfxMessageBox(_T("You have entered wrong characters!"), MB_ICONERROR);
		return;
	}

	iAlpha = _wtoi(sOurString);
	if ((iAlpha < 0) || (iAlpha > 255))
	{
		AfxMessageBox(_T("You have entered wrong number!"), MB_ICONERROR);
		return;
	}
	
	g_pWaterMarksView->m_iAlpha = iAlpha;
	g_pWaterMarksView->RedrawWindow();
}
//###############################################################################################################
bool COptionsForm::IsItNumber(const TCHAR& c)
{
	if ((c >= '0') && (c <= '9'))
		return true; 
	else
		return false;
}
//################################################################################################################
void COptionsForm::OnBnClickedButtonChooseTransparentColor()
{
	CColorDialog dlg(g_pWaterMarksView->m_cTransparentColor, CC_FULLOPEN);
	if (IDOK == dlg.DoModal())
	{
		try
		{
			g_pWaterMarksView->m_cTransparentColor = dlg.GetColor();
			g_pWaterMarksView->RedrawWindow();
		}
		catch (...)
		{
			AfxMessageBox(_T("Cannot change transparent color!"), MB_ICONERROR);
		}
	}
}
//################################################################################################################
void COptionsForm::OnBnClickedButtonHelp()
{
	CHelpDialog dlg;
	dlg.DoModal();
}
//#####################################################################################################################
void COptionsForm::OnBnClickedRadioCenter()
{
	m_RadioPage.SetCheck(0);
	m_RadioScale.SetCheck(0);
	g_pWaterMarksView->m_WatermarkPosition = E_WATERMARK_CENTER;
	g_pWaterMarksView->RedrawWindow();
}
//#####################################################################################################################
void COptionsForm::OnBnClickedRadioFitToPage()
{
	m_RadioCenter.SetCheck(0);
	m_RadioScale.SetCheck(0);
	g_pWaterMarksView->m_WatermarkPosition = E_WATERMARK_PAGE;
	g_pWaterMarksView->RedrawWindow();
}
//#####################################################################################################################
void COptionsForm::OnBnClickedRadioScaleToFit()
{
	m_RadioCenter.SetCheck(0);
	m_RadioPage.SetCheck(0);
	g_pWaterMarksView->m_WatermarkPosition = E_WATERMARK_FIT;
	g_pWaterMarksView->RedrawWindow();
}
//#####################################################################################################################
