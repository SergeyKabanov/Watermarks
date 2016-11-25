
// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "WaterMarks.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainFrame* g_pMainFrame;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
//############################################################################################################
// CMainFrame construction/destruction
//############################################################################################################
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	g_pMainFrame = this;

	m_clrBkgnd = LIGHT_LIGHT_GREEN;
	m_clrText = BLUE;
}
//############################################################################################################
CMainFrame::~CMainFrame()
{
}
//############################################################################################################
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

	//m_pOptionsForm = COptionsForm::CreateDynamic(this);
	//m_pOptionsForm->Init();

	//SetTitle(_T("WATERMARKS"));
	SetWindowText(_T("WATERMARKS"));
	return 0;
}
//############################################################################################################
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.hMenu = NULL;
	cs.style &= ~(LONG)FWS_ADDTOTITLE; // to get rid of "Untitled - ..."
	return TRUE;
}
//############################################################################################################
// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}
//############################################################################################################
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame message handlers
//############################################################################################################
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitterMain.CreateStatic(this, 1, 2))
	{
		//TRACE0("Failed to CreateStaticSplitter\n");
		AfxMessageBox(_T("CMainFrame: Failed to First Static Splitter\n"));
		return FALSE;
	}

	int cx = lpcs->cx / 2;
	int cy = lpcs->cy;

	if (!m_wndSplitterMain.CreateView(0, 0, RUNTIME_CLASS(COptionsForm), CSize(cx, cy), pContext))
	{
		//TRACE0("Failed to create first pane\n");
		AfxMessageBox(_T("CMainFrame: Failed to create first pane\n"));
		return FALSE;
	}

	if (!m_wndSplitterMain.CreateView(0, 1, RUNTIME_CLASS(CWaterMarksView), CSize(cx, cy), pContext))
	{
		//TRACE0("Failed to create first pane\n");
		AfxMessageBox(_T("CMainFrame: Failed to create first pane\n"));
		return FALSE;
	}
	
	SetActiveView((CView*)m_wndSplitterMain.GetPane(0, 1));
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	m_wndSplitterMain.SetColumnInfo(0, 200, 30);
	m_wndSplitterMain.RecalcLayout();
	return TRUE;
}
//############################################################################################################
void CMainFrame::OnAppExit()
{
	//AfxMessageBox(_T(""));
}
//#############################################################################################################
