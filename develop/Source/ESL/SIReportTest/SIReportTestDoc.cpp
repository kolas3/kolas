// SIReportTestDoc.cpp : implementation of the CSIReportTestDoc class
//

#include "stdafx.h"
#include "SIReportTest.h"

#include "SIReportTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestDoc

IMPLEMENT_DYNCREATE(CSIReportTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CSIReportTestDoc, CDocument)
	//{{AFX_MSG_MAP(CSIReportTestDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestDoc construction/destruction

CSIReportTestDoc::CSIReportTestDoc()
{
	// TODO: add one-time construction code here

}

CSIReportTestDoc::~CSIReportTestDoc()
{
}

BOOL CSIReportTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSIReportTestDoc serialization

VOID CSIReportTestDoc::Serialize(CArchive& ar)
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
// CSIReportTestDoc diagnostics

#ifdef _DEBUG
VOID CSIReportTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CSIReportTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestDoc commands
