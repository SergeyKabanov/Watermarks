
// WaterMarksDoc.cpp : implementation of the CWaterMarksDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaterMarks.h"
#endif

#include "WaterMarksDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWaterMarksDoc

IMPLEMENT_DYNCREATE(CWaterMarksDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaterMarksDoc, CDocument)
END_MESSAGE_MAP()


// CWaterMarksDoc construction/destruction

CWaterMarksDoc::CWaterMarksDoc()
{
	// TODO: add one-time construction code here

}

CWaterMarksDoc::~CWaterMarksDoc()
{
}

BOOL CWaterMarksDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

//#######################################################################################################################################
void CWaterMarksDoc::SetWindowTitle(CString strTitle)      // Sets base window title without recording status adornments
{
	m_sWindowTitle = strTitle;      // save as the base window title
	SetTitle(strTitle);             // update the displayed window title
}
//#######################################################################################################################################
void CWaterMarksDoc::SetTitle(CString strTitle)      // Sets base window title without recording status adornments
{
	CDocument::SetTitle(strTitle);
}
//#######################################################################################################################################
// CWaterMarksDoc serialization

void CWaterMarksDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWaterMarksDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWaterMarksDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWaterMarksDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWaterMarksDoc diagnostics

#ifdef _DEBUG
void CWaterMarksDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaterMarksDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWaterMarksDoc commands
