
// WaterMarks.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "WaterMarks.h"
#include "MainFrm.h"

#include "WaterMarksDoc.h"
#include "WaterMarksView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaterMarksApp

CWaterMarksApp* p_OurApp;

BEGIN_MESSAGE_MAP(CWaterMarksApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWaterMarksApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

// CWaterMarksApp construction
//############################################################################################################
CWaterMarksApp::CWaterMarksApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("WaterMarks.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	p_OurApp = this;
	m_bDoNotLoadFiles = false;
}
//############################################################################################################
// The one and only CWaterMarksApp object
CWaterMarksApp theApp;
//############################################################################################################
// CWaterMarksApp initialization
//############################################################################################################
BOOL CWaterMarksApp::InitInstance()
{
	CString sArguments;

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxInitRichEdit2();
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWaterMarksDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(COptionsForm));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
	
	// Initialize COM
	CoInitialize(nullptr);

	sArguments = GetCommandLine();
	GetFileNames(sArguments);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}
//#############################################################################################################
int CWaterMarksApp::ExitInstance()
{
	//TODO: handle additional resources you may have added

	AfxOleTerm(FALSE);
	GdiplusShutdown(m_pGdiToken);

	CWinApp::ExitInstance();

	// Uninitialize COM
	//CoUninitialize();

	return 0;
}
//###############################################################################################################
void CWaterMarksApp::GetFileNames(const CString & sInput)
{
	// First, we parse our string into the vector of words:

	TCHAR cCur;
	int i;
	bool bWord;
	CString sTemp;
	vector<CString> vWords;

	sTemp = "";
	vWords.clear();
	bWord = false;

	//for (it_string = sInput.begin(); it_string < sInput.end(); ++it_string)
	for (i = 0; i < (int)sInput.GetLength(); ++i)
	{
		cCur = sInput.GetAt(i);

		// word starts:
		if ((cCur != _T(' ')) && !bWord)
		{
			bWord = true;
			sTemp = cCur;
		}
		else
			// word continues:
			if ((cCur != _T(' ')) && bWord)
			{
				sTemp = sTemp + cCur;
			}
			else
				// word ends:
				if ((cCur == _T(' ')) && bWord)
				{
					bWord = false;
					vWords.push_back(sTemp);
					sTemp = "";
				}
	} // for (i = 0; i < sInput.length(); ++i)
	  // the last word:
	if (sTemp.GetLength()>0)
		vWords.push_back(sTemp);

	// Now we parsed the string into the vector of words

	if (vWords.size() < 3)
	{
		//AfxMessageBox(_T(""));
		m_bDoNotLoadFiles = true;
		return; // just start the program as usual
	}

	sFirstFileName = vWords[1];
	sSecondFileName = vWords[2];
}
//###############################################################################################################

// CWaterMarksApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CWaterMarksApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWaterMarksApp message handlers



