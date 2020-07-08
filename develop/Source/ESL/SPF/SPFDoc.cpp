// SPFDoc.cpp : implementation of the CSPFDoc class
//

#include "stdafx.h"
#include "SPF.h"

#include "SPFDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPFDoc

IMPLEMENT_DYNCREATE(CSPFDoc, CDocument)

BEGIN_MESSAGE_MAP(CSPFDoc, CDocument)
	//{{AFX_MSG_MAP(CSPFDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPFDoc construction/destruction

CSPFDoc::CSPFDoc()
{
	// TODO: add one-time construction code here

}

CSPFDoc::~CSPFDoc()
{
}

BOOL CSPFDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSPFDoc serialization

VOID CSPFDoc::Serialize(CArchive& ar)
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
// CSPFDoc diagnostics

#ifdef _DEBUG
VOID CSPFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CSPFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPFDoc commands
