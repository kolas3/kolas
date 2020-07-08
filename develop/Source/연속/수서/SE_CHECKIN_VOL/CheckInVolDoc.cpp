// CheckInVolDoc.cpp : implementation file
//

#include "stdafx.h"
#include "CheckInVolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckInVolDoc

IMPLEMENT_DYNCREATE(CCheckInVolDoc, CDocument)

CCheckInVolDoc::CCheckInVolDoc()
{
}

BOOL CCheckInVolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCheckInVolDoc::~CCheckInVolDoc()
{
}


BEGIN_MESSAGE_MAP(CCheckInVolDoc, CDocument)
	//{{AFX_MSG_MAP(CCheckInVolDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckInVolDoc diagnostics

#ifdef _DEBUG
VOID CCheckInVolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CCheckInVolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCheckInVolDoc serialization

VOID CCheckInVolDoc::Serialize(CArchive& ar)
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
// CCheckInVolDoc commands
