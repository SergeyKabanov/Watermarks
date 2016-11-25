#pragma once



// COptionsForm form view

class COptionsForm : public CFormView
{
	DECLARE_DYNCREATE(COptionsForm)

protected:
	COptionsForm();           // protected constructor used by dynamic creation
	virtual ~COptionsForm();

public:
	static COptionsForm* CreateDynamic(CWnd *pParent);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTIES };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	bool bFirstTime;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Init();
	bool IsItNumber(const TCHAR& c);

	CButton m_RadioCenter;
	CButton m_RadioPage;
	CButton m_RadioScale;

	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnBnClickedButtonLoadBackgroundImage();
	afx_msg void OnBnClickedButtonLoadWatermarkImage2();
	afx_msg void OnBnClickedButtonSetTransparencyLevel();
	afx_msg void OnBnClickedButtonChooseTransparentColor();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedRadioCenter();
	afx_msg void OnBnClickedRadioFitToPage();
	afx_msg void OnBnClickedRadioScaleToFit();
};


