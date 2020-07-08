// AcqGroupDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AcqGroupDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupDoc

IMPLEMENT_DYNCREATE(CAcqGroupDoc, CDocument)

CAcqGroupDoc::CAcqGroupDoc()
{
}

BOOL CAcqGroupDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAcqGroupDoc::~CAcqGroupDoc()
{
}


BEGIN_MESSAGE_MAP(CAcqGroupDoc, CDocument)
	//{{AFX_MSG_MAP(CAcqGroupDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupDoc diagnostics

#ifdef _DEBUG
VOID CAcqGroupDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CAcqGroupDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAcqGroupDoc serialization

VOID CAcqGroupDoc::Serialize(CArchive& ar)
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
// CAcqGroupDoc commands
