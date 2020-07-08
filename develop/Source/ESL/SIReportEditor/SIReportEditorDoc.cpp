// SIReportEditorDoc.cpp : implementation of the CSIReportEditorDoc class
//

#include "stdafx.h"
#include "SIReportEditor.h"

#include "SIReportEditorDoc.h"

#include "SIReportEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorDoc

IMPLEMENT_DYNCREATE(CSIReportEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSIReportEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CSIReportEditorDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorDoc construction/destruction

CSIReportEditorDoc::CSIReportEditorDoc()
{
	// TODO: add one-time construction code here

}

CSIReportEditorDoc::~CSIReportEditorDoc()
{
}

BOOL CSIReportEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorDoc serialization

VOID CSIReportEditorDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorDoc diagnostics

#ifdef _DEBUG
VOID CSIReportEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CSIReportEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorDoc commands

// For Argument
BOOL CSIReportEditorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString strFilePath;
	strFilePath.Format(_T("%s"), lpszPathName);

	if (strFilePath.GetLength() > 0) {
		CString strOpenAlias = strFilePath.Mid(strFilePath.ReverseFind(_T('\\'))+1);
		POSITION pos = GetFirstViewPosition();
		CSIReportEditorView* pView = ( CSIReportEditorView * )GetNextView( pos );
		
		if ( NULL != pView ) {
			pView->m_strOpenAlias = strOpenAlias;
		}
	}

	
	return TRUE;
}
